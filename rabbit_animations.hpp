#pragma once
#include "scene_graph.hpp"
#include "animation.hpp"
#include <array>

namespace akj{
	// little convenience struct
	struct cAnimatedNode
	{
		cAnimatedNode(cSceneGraphNode* node, cUnitQuat quat):mTargetRotation(quat), mNode(node){}
		cAnimatedNode(): mTargetRotation(),mNode(NULL){};
		~cAnimatedNode(){}
		cUnitQuat mTargetRotation;
		cSceneGraphNode* mNode;
	};

	class Rabbit;
	class cRabbitAnimation : public cAnimation
	{
	public:
		cRabbitAnimation(Rabbit& parent,const char* name, eAnimationDirection direction, eAnimationType type)
			: cAnimation( name)
			, mType(type)
			, mDirection(direction)
			, mParent(parent)
		{}
		//keep the same animation, but possibly change direction or jump/smooth
		void Restart(eAnimationDirection direction, eAnimationType type)
		{
			mDirection = direction;
			mType = type;
			reset();
			Init();
		}
		virtual void DoFrame(){};
		virtual void Init(){};
		// are we in the forward or backwards phase?
		inline bool IsPerturbing() const
		{
			return mDirection != cAnimation::RESTORE && mDirection != cAnimation::RESTORE_REPEAT;
		}
		eAnimationType GetType() const { return mType;}
		eAnimationDirection GetDirection() const {return mDirection;}
	
		// override the duration in jump mode
		virtual void SetDuration(float duration)
		{
			if(mType == cAnimation::JUMP)
			{
				mDuration = 0.0f;
			}
			else
			{
				mDuration = duration;
			}
		}
		protected:
		Rabbit& mParent;
		eAnimationDirection mDirection;
		eAnimationType mType;
	};


	//////////////////////////////////////////////////////////////////////////
	class cHeadNodAnimation : public cRabbitAnimation
	{
	public:
		cHeadNodAnimation(Rabbit& parent, eAnimationDirection direction, eAnimationType type);
		virtual void DoFrame();
		virtual void Init();
	private:
		cSceneGraphNode* mMidNeck;
		cSceneGraphNode* mTopNeck;
		cSceneGraphNode* mBaseNeck;
	};


	//////////////////////////////////////////////////////////////////////////
	class cEarWiggleAnimation : public cRabbitAnimation
	{
	public:
		cEarWiggleAnimation (Rabbit& parent, bool is_left, eAnimationDirection direction, eAnimationType type);
		virtual void DoFrame();
		virtual void Init();
	private:
		const bool mIsLeft;
		cSceneGraphNode* mEar;
		cUnitQuat mTargetRot;
	};


	//////////////////////////////////////////////////////////////////////////
	class cHindLegRaiseAnimation : public cRabbitAnimation
	{
	public:
		cHindLegRaiseAnimation (Rabbit& parent, bool is_left, eAnimationDirection direction, eAnimationType type);
		virtual void Init();
		virtual void DoFrame();

	private:
		const bool mIsLeft;
		std::vector<cAnimatedNode> mNodes;
	};


	//////////////////////////////////////////////////////////////////////////
	class cFrontLegRaiseAnimation : public cRabbitAnimation
	{
	public:
		cFrontLegRaiseAnimation (Rabbit& parent, bool is_left, eAnimationDirection direction, eAnimationType type);
		virtual void Init();
		virtual void DoFrame();

	private:
		const bool mIsLeft;
		std::vector<cAnimatedNode> mNodes;
	};


	//////////////////////////////////////////////////////////////////////////
	class cBodyCurlAnimation : public cRabbitAnimation
	{
	public:
		cBodyCurlAnimation (Rabbit& parent, eAnimationDirection direction, eAnimationType type);
		virtual void Init();
		virtual void DoFrame();

	private:
		cCoord3 mSpineTarget;
		cCoord3 mSpineInitialPos;
		cSceneGraphNode* mLeftHeel;
		cSceneGraphNode* mRightHeel;
		cCoord3 mFootDirection;
		cCoord3 mFootPosition;
		std::vector<cAnimatedNode> mNodes;
	};


	//////////////////////////////////////////////////////////////////////////
	class cRearUpAnimation : public cRabbitAnimation
	{
	public:
		cRearUpAnimation (Rabbit& parent, eAnimationDirection direction, eAnimationType type);
		virtual void Init();
		virtual void DoFrame();

	private:
		std::vector<cAnimatedNode> mNodes;
		cSceneGraphNode* mLeftHeel;
		cSceneGraphNode* mRightHeel;
		cCoord3 mFootDirection;
		cCoord3 mFootPosition;
		cCoord3 mSpineTarget;
		cCoord3 mSpineInitialPos;
	};

	//////////////////////////////////////////////////////////////////////////
	class cJumpAnimation : public cRabbitAnimation
	{
	public:
		cJumpAnimation (Rabbit& parent, eAnimationDirection direction, eAnimationType type);
		virtual void Init();

		virtual void DoFrame();

		void LeapPhase( const float fraction );
		

	private:

		std::vector<cAnimatedNode> mCrouchNodes;
		std::vector<cAnimatedNode> mLeapNodes;
		cSceneGraphNode* mLeftHeel;
		cSceneGraphNode* mRightHeel;
		cSceneGraphNode* mLeftToe;
		cSceneGraphNode* mRightToe;
		cCoord3 mFootDirection;
		cCoord3 mFootPosition;
		cCoord3 mToePosition;
		cCoord3 mLeapSpineTarget;
		cCoord3 mCrouchSpineTarget;
		cCoord3 mSpineInitialPos;
		cCoord3 mHipHeight;
		const float mCrouchPhase;
		const float mLeapPhase;
		const float mDesiredDuration;
		float mLastFraction;
	};


	//////////////////////////////////////////////////////////////////////////
	class cEllipseToCubeAnimation : public cRabbitAnimation
	{
	public:
		cEllipseToCubeAnimation(Rabbit& parent, eAnimationDirection direction, eAnimationType type);
		virtual void DoFrame();
		virtual void Init();
	private:
		cCoord3 mAlternateShape;
	};
}// end namespace akj
