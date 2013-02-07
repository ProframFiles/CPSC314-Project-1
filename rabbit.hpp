#pragma once
#include "cs314_math.hpp"
#include "scene_graph.hpp"
#include "rabbit_animations.hpp"
#include <memory>
#include <cassert>


namespace akj{


class Rabbit : public cSceneGraphNode
{
public:
	enum eRabbitState
	{
		IDLE,
		HEAD_NOD,
		LIMB_RAISE_RIGHT_HIND,
		LIMB_RAISE_LEFT_HIND,
		LIMB_RAISE_RIGHT_FORE,
		LIMB_RAISE_LEFT_FORE,
		EAR_RAISE_RIGHT,
		EAR_RAISE_LEFT,
		REAR_UP,
		BODY_CURL,
		JUMP,
		ELLIPSETRANS,
		LAST_ANIM_ENTRY
	};
	Rabbit(const char* name):cSceneGraphNode(name)
	{
		mSkeleton = CreateSkeleton();
		assert(mSkeleton);
		cSceneGraphNode* neck_end = mSkeleton->FindDescendant("neck end");
		assert(neck_end);
		mHead = CreateHead(neck_end);
		mSpineBase = mSkeleton->FindDescendant("spine base");
		mSpineEnd = mSkeleton->FindDescendant("spine end");
		assert(mSpineBase && mSpineEnd);
		mAnimations.resize(LAST_ANIM_ENTRY);
	};
	~Rabbit(){};
	tBoneNode* GetSkeletonBase() 
	{
		return mSkeleton.get();
	}
	inline std::vector<cEllipseCubedNode*>::iterator BeginEllipses() 
	{
		return mEllipseNodes.begin();
	}
	inline std::vector<cEllipseCubedNode*>::iterator EndEllipses() 
	{
		return mEllipseNodes.end();
	}
	void SetAnimation(eRabbitState anim_state, cAnimation::eAnimationDirection direction, cAnimation::eAnimationType mode);
	void SetReverseAnimation(eRabbitState anim_state, cAnimation::eAnimationType mode);
	bool Update(float elapsed_time);
	void CurlSpineTowards(const cUnitQuat& rot);
	virtual void Draw();
	cSceneGraphNode* GetSpineEnd()
	{
		return mSpineEnd;
	}
	//relative to the base of the pelvis
	// acts as the rotation + scaling center
	cCoord3 GetCenterOfMass() const
	{
		return cCoord3(0.0f, 1.0f, -1.5f);
	}
	
private:
	std::unique_ptr<tBoneNode> CreateSkeleton();
	tBoneNode* CreateHead(cSceneGraphNode* neck_end);
	tBoneNode* CreateBones( tBoneNode* parent,const cCoord3& orientation,const cCoord3* coords, float* thickness, const char** names, int num_bones, float lateral = 1.0f);
	void CreateCurvedPoints(cCoord3* points,int num_points, const cCoord3& start, const cCoord3& end, const cCoord3& rotation_center, float factor = 1.0f);
	void CreateLeftAndRightShells(tBoneNode* base, const char* name_1, const char* name_2 , const cCoord3& start_adjust, const cCoord3& end_adjust, const cCoord2& aspect, int num_strips, int num_sections);

	std::unique_ptr<tBoneNode> mSkeleton;
	cSceneGraphNode* mSpineBase;
	cSceneGraphNode* mSpineEnd;
	std::vector<cEllipseCubedNode*> mEllipseNodes;
	tBoneNode* mHead;
	std::vector<std::unique_ptr<cRabbitAnimation> > mAnimations;
	friend class cRabbitAnimation;
};

}
