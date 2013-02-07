#pragma once
#include <vector>
#include "akj_ogl.h"
#include "cs314_math.hpp"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct cCubeDescriptor
{
	cCubeDescriptor(){}
	cCubeDescriptor(const cCoord3& location, const cCoord3& shape, const cUnitQuat& orientation)
		: mLocation(location)
		, mShape(shape)
		, mOrientation(orientation)
		, mDeviation(cCoord3::Zeros())
	{}
	~cCubeDescriptor(){}
	cCoord3 mLocation;
	cCoord3 mDeviation;
	cCoord3 mShape;
	cUnitQuat mOrientation;
};

class cDrawableGLCube
{
public:
	void Draw(const cCoord3& scale,const cCoord3& translation, const cUnitQuat& rotation, const cCoord3& shape, const cCoord3& org)
	{
		glTranslatef(translation.x, translation.y, translation.z);
		akj::glRotateq(rotation);
		glPushMatrix();
		glTranslatef(org.x, org.y, org.z);
		glScalef(shape.x, shape.y, shape.z);
		akj::glDrawCube();
		glPopMatrix();
		glScalef(scale.x, scale.y, scale.z);
	}
};

class cSceneGraphNode
{
public:
	cSceneGraphNode(const char* name)
		:mName(name)
		,mParent(NULL)
		,mTranslation()
		,mScale(1.0f, 1.0f, 1.0f)
		,mRotation()
		, mCurrentChild(0)
	{};

	cSceneGraphNode(const char* name, cSceneGraphNode* parent)
		:mName(name)
		,mParent(parent)
		,mTranslation()
		,mScale(1.0f, 1.0f, 1.0f)
		,mRotation()
		,mCurrentChild(0)
	{
		//printf("created node %s\n", mName);
	};
	virtual ~cSceneGraphNode(){};

	const char* GetName() const
	{
		return mName.c_str();
	}
	void SetTranslation(const cCoord3& trans)
	{
		mTranslation = trans;
	}
	void AddTranslation(const cCoord3& trans)
	{
		mTranslation = trans + mTranslation;
	}
	const cCoord3& GetTranslation() const 
	{
		return mTranslation;
	}
	void SetScale(const cCoord3& scale)
	{
		mScale = scale;
	}
	void SetScale(float scale)
	{
		mScale = scale*cCoord3(1.0f, 1.0f, 1.0f);
	}
	void MultiplyScale(const cCoord3& scale_mult)
	{
		mScale.x *= scale_mult.x;
		mScale.y *= scale_mult.y;
		mScale.z *= scale_mult.z;
	}
	void MultiplyScale(const float scale_mult)
	{
		mScale.x *= scale_mult;
		mScale.y *= scale_mult;
		mScale.z *= scale_mult;
	}
	cCoord3 GetScale() const
	{
		return mScale;
	}
	void SetRotation(const cUnitQuat& quat)
	{
		mRotation = quat;
	}
	void SetRotationFromWorldVector(const cCoord3& vec)
	{
		const cCoord3 local_vec = GetAccumBaseRotation().rev_rotate(vec);
		mRotation = cUnitQuat(cCoord3::ZAxis(), local_vec );
	}
	virtual cUnitQuat GetBaseOrientation() const
	{
		return cUnitQuat();
	}
	cUnitQuat GetAccumOrientation() const
	{
		cSceneGraphNode* parent = GetParent();
		cUnitQuat rot = GetBaseOrientation();
		while(parent)
		{
			//printf("parent %s total rotation here\n", parent->GetName());
			rot = rot*parent->GetBaseOrientation();
			parent = parent->GetParent();
		}
		return rot;
	}
	cUnitQuat GetAccumBaseRotation() const
	{
		cSceneGraphNode* parent = GetParent();
		cUnitQuat rot = GetBaseOrientation();
		while(parent)
		{
			//printf("parent %s total rotation here\n", parent->GetName());
			rot = parent->GetBaseOrientation()*parent->GetRotation()*rot;
			parent = parent->GetParent();
		}
		return rot;
	}
	cCoord3 GetAccumPosition() const
	{
		const cSceneGraphNode* parent = GetParent();
		cCoord3 position = GetTranslation();
		while(parent)
		{
			
			position = position/parent->GetScale();
			position = (parent->GetBaseOrientation()*parent->GetRotation()).rotate(position);
			position = position + parent->GetTranslation();
			//printf("parent %s total rotation here\n", parent->GetName());
			
			parent = parent->GetParent();
		}
		return position;
	}

	cCoord3 GetAccumDeviation() const
	{
		const cSceneGraphNode* parent = GetParent();
		cCoord3 position = GetTranslation();
		cCoord3 dev_position = position;
		while(parent)
		{

			position = position/parent->GetScale();
			dev_position = dev_position/parent->GetScale();
			position = (parent->GetBaseOrientation()).rotate(position);
			dev_position = (parent->GetRotation()*parent->GetBaseOrientation()).rotate(dev_position);
			position = position + parent->GetTranslation();
			dev_position = dev_position + parent->GetTranslation();
			//printf("parent %s total rotation here\n", parent->GetName());

			parent = parent->GetParent();
		}
		return dev_position - position;
	}
	cUnitQuat GetRotation() const
	{
		return mRotation;
	}
	void AddRotation(const cUnitQuat& quat)
	{
		mRotation = mRotation*quat;
	}

	virtual void Draw() = 0;
	
	virtual cSceneGraphNode* GetParent() const
	{
		return mParent;
	};
	virtual void SetParent(cSceneGraphNode* parent){
		mParent  = parent;
	};
	virtual void BeginChildren()
	{
		mCurrentChild = 0;
	}
	virtual cSceneGraphNode* NextChild()
	{
		if( mCurrentChild < mChildren.size())
		{
			++mCurrentChild;
			return mChildren[mCurrentChild - 1].get();
		}
		return NULL;
	}
	bool IsChildOf(cSceneGraphNode* node_pointer) const
	{
		const cSceneGraphNode* meta_parent = GetParent();
		while (meta_parent)
		{
			if(meta_parent == node_pointer)
			{
				return true;
			}
			meta_parent = meta_parent->GetParent();
		}
		return false;
	}
	const cSceneGraphNode* GetRootNode() const
	{
		const cSceneGraphNode* meta_parent = this;
		while (meta_parent)
		{
			meta_parent = meta_parent->GetParent();
		}
		return meta_parent;
	}
	bool IsNameEqual(const char* name)
	{
		return mName == name;
	}
	cSceneGraphNode* FindDescendant(const char* name)
	{
		if(IsNameEqual(name))
		{
			return this;
		}
		cSceneGraphNode* ret = NULL;
		BeginChildren();
		for(cSceneGraphNode* node = NextChild(); node != NULL; node = NextChild())
		{
			ret = node->FindDescendant(name);
			if(ret != NULL)
			{
				break;
			}
		}
		return ret;
	}
	template <class tChild>
	tChild* addChild(const char* name)
	{
		tChild* child_ptr = new tChild(name, this);
		mChildren.emplace_back(child_ptr);
		return child_ptr;
	}
	
protected:
	std::string mName;
	cCoord3 mTranslation;
	cCoord3 mScale;
	cUnitQuat mRotation;
	cSceneGraphNode* mParent;
	std::vector<std::unique_ptr<cSceneGraphNode> > mChildren;
	unsigned int mCurrentChild;

};

class cNodeLink 
{
public:
	cNodeLink():mAttachedNode(NULL){}
	cNodeLink(cSceneGraphNode* attached_node,const cCoord3& attachment_point)
		:mAttachedNode(attached_node)
		, mAttachmentPoint(attachment_point)
	{};
	~cNodeLink(){};
	const cSceneGraphNode* GetAttachedNode() const
	{
		return mAttachedNode;
	};
	const cCoord3& GetAttachmentPoint() const
	{
		return mAttachmentPoint;
	}
private:
	cSceneGraphNode* mAttachedNode;
	cCoord3 mAttachmentPoint;
};


template <class tCubeDrawType>
class mTransformedCubeNode: public cSceneGraphNode
{
public:
	mTransformedCubeNode(const char* name, cSceneGraphNode* parent)
		: cSceneGraphNode(name, parent)
		, mShape(cCoord3::Ones())
		, mOrigin(cCoord3::Zeros())
	{};
	virtual ~mTransformedCubeNode()
	{
	};
	void SetBaseOrientation(const cUnitQuat& orientation)
	{
		mBaseOrientation = orientation;
	}
	virtual cUnitQuat GetBaseOrientation() const
	{
		return mBaseOrientation;
	}
	void SetShape(const cCoord3& shape)
	{
		mShape = shape;
	}
	void SetOrigin(const cCoord3& origin)
	{
		mOrigin = origin;
	}
	const cCoord3& GetShape() const
	{
		return mShape;
	}
	virtual void Draw()
	{
		glPushMatrix();
		tCubeDrawType().Draw(mScale, mTranslation, mBaseOrientation*mRotation, mShape, mOrigin);
		
		BeginChildren();
		for(cSceneGraphNode* node = NextChild(); node != NULL; node = NextChild())
		{
			//push and pop when desending into the heirarchy
			node->Draw();
		}
		glPopMatrix();
	};


protected:
	cCoord3 mShape;
	cCoord3 mOrigin;
	cUnitQuat mBaseOrientation;
};
typedef mTransformedCubeNode<cDrawableGLCube> tBoneNode;

class cEllipseCubedNode: public tBoneNode
{
public:
	cEllipseCubedNode(const char* name, cSceneGraphNode* parent)
		: mTransformedCubeNode(name, parent)
		, mEllipseAxis(cCoord3::Zeros())
		, mEllipseEndPoint()
		, mEllipseFactor(1.0f)
		, mRandomFactor(0.0f)
		, mNotEllipseShape( 0.4f, 0.01f, 0.01f )
	{
		mShape = cCoord3::Zeros();
	};
	void SetEllipseFactor(float factor)
	{
		mEllipseFactor = factor;
	}
	void SetNotEllipseShape(const cCoord3& shape)
	{
		mNotEllipseShape = shape;
	}
	void SetRandomFactor(float factor)
	{
		mRandomFactor = factor;
	}
	virtual void Draw()
	{
		glPushMatrix();
		cDrawableGLCube().Draw(mScale, mTranslation, mBaseOrientation*mRotation, mShape, mOrigin);

		CheckAttachedNodes();

		for (auto i = mSimpleChildren.begin(); i != mSimpleChildren.end(); ++i)
		{
			glPushMatrix();
			const float randfact1 = mRandomFactor*(0.5f*(static_cast<float>(rand())/RAND_MAX) - 0.25f);
			const float randfact2 = mRandomFactor*(0.5f*(static_cast<float>(rand())/RAND_MAX) - 0.25f);
			//TODO turn this back on
			i->mDeviation = 0.01f*cCoord3(randfact2, 0.0f,randfact1) + i->mDeviation;
			cDrawableGLCube().Draw(cCoord3::Ones(), i->mLocation + i->mDeviation, cUnitQuat::ZY().scale(randfact2)*i->mOrientation*cUnitQuat::ZX().scale(randfact1), (1.0f-mEllipseFactor)*mNotEllipseShape + mEllipseFactor*i->mShape, cCoord3::Zeros() );
			glPopMatrix();
		}

		BeginChildren();
		for(cSceneGraphNode* node = NextChild(); node != NULL; node = NextChild())
		{
			//push and pop when desending into the heirarchy
			node->Draw();
		}
		glPopMatrix();
	};
	void CreateEllipsoidAlongZ(cSceneGraphNode* end_node, cCoord3 start_adjust, cCoord3 end_adjust, cCoord2 xy_size, int num_strips, int num_sections)
	{
		const float rand_strength = 0.0f;
		// generate num_sections + 1 points
		// this gets us n sections
		mEllipseEndPoint = cNodeLink(end_node, cCoord3::Zeros());
		// each line is located between consecutive points, with a length of the difference between them
		const cUnitQuat existing_orientation = end_node ? GetAccumOrientation() : cUnitQuat();
		const cCoord3 start = end_node ? GetAccumPosition() : cCoord3::Zeros();
		const cCoord3 end =  end_node ? end_node->GetAccumPosition() : cCoord3::Zeros();
		const cCoord3 line = (end + end_adjust - (start+start_adjust));
		const cCoord3 current_line = existing_orientation.rev_rotate(line);
		const cUnitQuat new_orientation(cCoord3::ZAxis(), current_line);
		const cCoord3 new_line = (existing_orientation*new_orientation).rotate(line.length()*cCoord3::ZAxis());
		const cCoord3 rotated_adjust = (existing_orientation*new_orientation).rev_rotate(start_adjust);
		SetBaseOrientation(new_orientation);
		mEllipseAxis = new_orientation.rev_rotate(current_line);
		mEndOffset = end_adjust;
		mStartOffset = start_adjust;
		const float length = line.length(); 
		float last_z = 0.0f;
		float last_t = 0.0f;
		float last_size = 0.0f;
		float last_circ = 0.0f; 
		for (int section = 1; section < num_sections+1; section++)
		{
			const float this_t = (static_cast<float>(section)/num_sections)*AKJ_PI;
			const float this_z = (length) * 0.5*(1.0-cosf(this_t));
			const float this_size = sinf(this_t);
			const float this_circ = EllipseCircumference(this_size*xy_size.x, this_size*xy_size.y );
			float first_mean_size = (last_size+this_size)*0.5f;
			cCoord3 last_middle = cCoord3(first_mean_size*xy_size.x,0.0f,(last_z+this_z)*0.5);
			for (int strip = 1; strip < num_strips+1; strip++)
			{
				const float strip_t = (static_cast<float>(strip)/num_strips)*2.0*AKJ_PI;
				const float x_factor = cosf(strip_t);
				const float y_factor = sinf(strip_t);
				const cCoord3 from = cCoord3(last_size*xy_size.x*x_factor, last_size*xy_size.y*y_factor, last_z );
				const cCoord3 to = cCoord3(this_size*xy_size.x*x_factor, this_size*xy_size.y*y_factor, this_z);
				const cCoord3 location = 0.5f*(from+to)+rotated_adjust;
				const cCoord3 this_middle = 0.5f*(from+to);
				const float width = (last_middle - this_middle).length();
				last_middle = this_middle;
				const cUnitQuat rotation = cUnitQuat( cCoord3::ZAxis(), to-from )*cUnitQuat(cCoord3::XAxis(), cCoord3(x_factor*xy_size.y, y_factor*xy_size.x, 0.0f)) ;
				const cCoord3 aspect(0.01f, width, 1.00f*(to-from).length());
				AddSimpleChild(cCubeDescriptor(location, aspect,rotation ));
			}
			last_circ = this_circ;
			last_z = this_z;
			last_t = this_t;
			last_size = this_size;
		}
	}


protected:
	virtual void CheckAttachedNodes()
	{
		if(NULL != mEllipseEndPoint.GetAttachedNode())
		{
			const cCoord3 w_endpoint_pos = mEllipseEndPoint.GetAttachedNode()->GetAccumPosition()+mEndOffset;
			const cCoord3 w_my_pos = GetAccumPosition();
			const cCoord3 line = w_endpoint_pos - w_my_pos;

			//rotated from local z
			const cUnitQuat accum_rot = GetAccumBaseRotation();
			const cCoord3 w_rot_end = accum_rot.rev_rotate(line);
			cUnitQuat new_rot(mEllipseAxis, w_rot_end);
			SetRotation(new_rot );
			const float scale_factor = (line-mStartOffset).length()/mEllipseAxis.length();
			const float scale_factor_sqrt = sqrtf(scale_factor);
			SetScale(cCoord3( 1.0/scale_factor_sqrt, 1.0/scale_factor_sqrt, scale_factor));
		}
	
	}
	void AddSimpleChild(const cCubeDescriptor& cube)
	{
		mSimpleChildren.push_back(cube);
	}
	float mEllipseFactor;
	cCoord3 mNotEllipseShape;
	std::vector<cCubeDescriptor> mSimpleChildren;
	cNodeLink mEllipseEndPoint;
	cCoord3 mEllipseAxis;
	cCoord3 mEndOffset;
	cCoord3 mStartOffset;
	float mRandomFactor;
};


