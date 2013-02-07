#include "rabbit_animations.hpp"
#include "rabbit.hpp"


namespace akj{

cHeadNodAnimation::cHeadNodAnimation( Rabbit& parent, eAnimationDirection direction, eAnimationType type )
	: cRabbitAnimation(parent, "head nod", direction, type)

{
	Init();
}
void cHeadNodAnimation::Init()
{
	SetDuration(0.3f);
	// cache these guys so we don't have to look every frame
	mBaseNeck = mParent.GetSkeletonBase()->FindDescendant("neck 1");
	mMidNeck = mBaseNeck->FindDescendant("neck 3");
	mTopNeck = mMidNeck->FindDescendant("rabbit skull");
}

void cHeadNodAnimation::DoFrame()
{
	const float fraction = IsPerturbing() ? FractionComplete() : 1.0f - FractionComplete();
	const float scale = 0.35f*(1.0f-cosf(1.0f*AKJ_PI*fraction));
	const cUnitQuat rot = cUnitQuat::ZY();
	const cUnitQuat rot_xy = cUnitQuat::XY();
	mMidNeck->SetRotation(rot.scale(0.4f*scale));
	mBaseNeck->SetRotation(rot.scale(0.2f*scale)*rot_xy.scale(0.3f*scale));
	mTopNeck->SetRotation(rot.scale(0.3f*scale)*rot_xy.scale(0.3f*scale));
}


//////////////////////////////////////////////////////////////////////////
cEllipseToCubeAnimation::cEllipseToCubeAnimation( Rabbit& parent, eAnimationDirection direction, eAnimationType type )
		: cRabbitAnimation(parent, "ellipse to cube", direction, type)
{
	Init();
}
void cEllipseToCubeAnimation::Init()
{
	SetDuration(2.0f);
	mAlternateShape = cCoord3( 0.02f, 0.02f, 0.02f );
	//mAlternateShape = cCoord3( 0.0f, 0.0f, 0.0f );
}

void cEllipseToCubeAnimation::DoFrame()
{
	const float fraction = IsPerturbing() ? 1.0f - FractionComplete() : FractionComplete();
	for (auto i = mParent.BeginEllipses(); i != mParent.EndEllipses();++i)
	{
		(*i)->SetRandomFactor(1.0f-fraction);
		(*i)->SetNotEllipseShape(mAlternateShape);
		(*i)->SetEllipseFactor(fraction);
	}
}



//////////////////////////////////////////////////////////////////////////
cEarWiggleAnimation::cEarWiggleAnimation( Rabbit& parent, bool is_left, eAnimationDirection direction, eAnimationType type )
	: mIsLeft(is_left)
	, cRabbitAnimation(parent, "ear wiggle", direction, type)
{
	Init();
}
void cEarWiggleAnimation::Init()
{
	SetDuration(0.3f);
	const cCoord3 target(-0.2f, -0.8f, -0.3f);
	if(mIsLeft)
	{
		mEar = mParent.GetSkeletonBase()->FindDescendant("left ear");
		mTargetRot = cUnitQuat(cCoord3::YAxis(), cCoord3(-1.0f, 1.0f, 1.0f)*target );
	}
	else
	{
		mEar = mParent.GetSkeletonBase()->FindDescendant("right ear");
		mTargetRot = cUnitQuat(cCoord3::YAxis(), target );
	}
}
void cEarWiggleAnimation::DoFrame()
{
	const float fraction = IsPerturbing() ? FractionComplete() : 1.0f - FractionComplete();
	const float scale = 0.35f*(1.0f-cosf(1.0f*AKJ_PI*fraction));
	mEar->SetRotation(mTargetRot.scale(scale));
}


//////////////////////////////////////////////////////////////////////////
cHindLegRaiseAnimation::cHindLegRaiseAnimation( Rabbit& parent, bool is_left, eAnimationDirection direction, eAnimationType type )
	: mIsLeft(is_left)
	, cRabbitAnimation(parent, "hind leg raise", direction, type)
{
	Init();
}

void cHindLegRaiseAnimation::Init()
{
	SetDuration(1.0f);
	mNodes.clear();
	if(mIsLeft)
	{
		mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left femur"), cUnitQuat::ZX().scale(0.0f)*cUnitQuat::ZY().scale(0.35f));
		mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left tibia"), cUnitQuat::ZY().scale(0.75f)*cUnitQuat::ZX().scale(0.45f));
		mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left hindfoot"), cUnitQuat::YZ().scale(0.26f)*cUnitQuat::XZ().scale(0.3f));
		mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left forefoot"), cUnitQuat::ZX().scale(0.3f));
	}
	else
	{
		mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right femur"), cUnitQuat::XZ().scale(0.0f)*cUnitQuat::ZY().scale(0.35f));
		mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right tibia"), cUnitQuat::ZY().scale(0.75f)*cUnitQuat::XZ().scale(0.45f));
		mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right hindfoot"), cUnitQuat::YZ().scale(0.26f)*cUnitQuat::ZX().scale(0.3f));
		mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right forefoot"), cUnitQuat::XZ().scale(0.3f));
	}

}

void cHindLegRaiseAnimation::DoFrame()
{
	const float fraction = IsPerturbing() ? FractionComplete() : 1.0f - FractionComplete();
	const float scale = 0.5f*(1.0f-cosf(1.0f*AKJ_PI*fraction));
	for (auto i = mNodes.begin(); i != mNodes.end() ; ++i )
	{
		i->mNode->SetRotation(i->mTargetRotation.scale(scale));
	}
}


//////////////////////////////////////////////////////////////////////////
cFrontLegRaiseAnimation::cFrontLegRaiseAnimation( Rabbit& parent, bool is_left, eAnimationDirection direction, eAnimationType type )
	: mIsLeft(is_left)
	, cRabbitAnimation(parent, "front leg raise", direction, type)
{
	Init();
}


void cFrontLegRaiseAnimation::Init()
{
	SetDuration(0.7f);
	mNodes.clear();
	if(mIsLeft)
	{
		mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left elbow"), cUnitQuat::XZ().scale(0.18f)*cUnitQuat::YZ().scale(0.18f));
		mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left wrist"), cUnitQuat::XZ().scale(0.18f)*cUnitQuat::YZ().scale(0.18f));
		mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left palm"), cUnitQuat::ZX().scale(0.46f)*cUnitQuat::ZY().scale(0.46f));

	}
	else
	{
		mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right elbow"), cUnitQuat::ZX().scale(0.18f)*cUnitQuat::YZ().scale(0.18f));
		mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right wrist"), cUnitQuat::ZX().scale(0.18f)*cUnitQuat::YZ().scale(0.18f));
		mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right palm"), cUnitQuat::XZ().scale(0.46f)*cUnitQuat::ZY().scale(0.46f));
	}
}

void cFrontLegRaiseAnimation::DoFrame()
{
	const float fraction = IsPerturbing() ? FractionComplete() : 1.0f - FractionComplete();
	const float scale = 0.5f*(1.0f-cosf(1.0f*AKJ_PI*fraction));
	for (auto i = mNodes.begin(); i != mNodes.end() ; ++i )
	{
		i->mNode->SetRotation(i->mTargetRotation.scale(scale));
	}
}


//////////////////////////////////////////////////////////////////////////
cBodyCurlAnimation::cBodyCurlAnimation( Rabbit& parent, eAnimationDirection direction, eAnimationType type )
	: cRabbitAnimation(parent, "body curl", direction, type)
{

	mNodes.clear();
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left elbow"), cUnitQuat::XZ().scale(0.0f)*cUnitQuat::ZY().scale(0.10f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right elbow"), cUnitQuat::ZX().scale(0.0f)*cUnitQuat::ZY().scale(0.10f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left wrist"), cUnitQuat::YZ().scale(0.54f)*cUnitQuat::XZ().scale(0.48f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right wrist"), cUnitQuat::YZ().scale(0.54f)*cUnitQuat::ZX().scale(0.48f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left palm"), cUnitQuat::ZX().scale(0.28f)*cUnitQuat::ZY().scale(0.28f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right palm"), cUnitQuat::XZ().scale(0.28f)*cUnitQuat::ZY().scale(0.28f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 2"),cUnitQuat(cCoord3::YAxis(), cCoord3::ZAxis()) );
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 4"),cUnitQuat::ZY().scale(1.3) );
	mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left femur"), cUnitQuat::ZY().scale(0.1f));
	mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left tibia"), cUnitQuat::ZY().scale(0.10f)*cUnitQuat::XZ().scale(0.0f));
	mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right femur"), cUnitQuat::ZY().scale(0.1f));
	mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right tibia"), cUnitQuat::ZY().scale(0.10f)*cUnitQuat::ZX().scale(0.0f));

	mLeftHeel = mParent.GetSkeletonBase()->FindDescendant("left hindfoot");
	mRightHeel = mParent.GetSkeletonBase()->FindDescendant("right hindfoot");
	mSpineInitialPos = mParent.GetSpineEnd()->GetAccumPosition();
	mSpineTarget = cCoord3(0.0, 0.0, -4.0);
	Init();
}

void cBodyCurlAnimation::Init()
{
	SetDuration(1.5f);
	cSceneGraphNode* right_toes = mParent.GetSkeletonBase()->FindDescendant("right forefoot");
	mFootPosition = mRightHeel->GetAccumPosition();
	mFootDirection = (right_toes->GetAccumPosition()-mFootPosition).normalized();
}

void cBodyCurlAnimation::DoFrame()
{
	const float fraction = IsPerturbing() ? FractionComplete() : 1.0f - FractionComplete();
	const float scale = 0.5f*(1.0f-cosf(1.0f*AKJ_PI*fraction));

	for (auto i = mNodes.begin(); i != mNodes.end() ; ++i )
	{
		i->mNode->SetRotation(i->mTargetRotation.scale(scale));
	}
	mParent.CurlSpineTowards(cUnitQuat(mSpineInitialPos, mSpineTarget).scale(scale));
	const cCoord3 pos_heel = mRightHeel->GetAccumPosition();
	cCoord3 adjust = cCoord3(0.0f, -1.0f, -1.0f)*( pos_heel - mFootPosition);
	mParent.GetSkeletonBase()->AddTranslation(adjust);
	mRightHeel->SetRotationFromWorldVector(mFootDirection);
	mLeftHeel->SetRotationFromWorldVector(cCoord3(-1.0f, 1.0f, 1.0f )*mFootDirection);

	//printf("scale: %f \n",scale);
}


//////////////////////////////////////////////////////////////////////////
cRearUpAnimation::cRearUpAnimation( Rabbit& parent, eAnimationDirection direction, eAnimationType type )
	: cRabbitAnimation(parent, "rear up", direction, type)
{
	
	mNodes.clear();
	// arms hang to side
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left shoulder"), cUnitQuat::ZY().scale(0.58f)*cUnitQuat::ZX().scale(0.2f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right shoulder"), cUnitQuat::ZY().scale(0.58f)*cUnitQuat::XZ().scale(0.2f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left elbow"), cUnitQuat::YZ().scale(0.25f)*cUnitQuat::XZ().scale(0.05f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right elbow"), cUnitQuat::YZ().scale(0.25f)*cUnitQuat::ZX().scale(0.05f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left wrist"), cUnitQuat::ZX().scale(0.6f)*cUnitQuat::ZY().scale(0.15f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right wrist"), cUnitQuat::XZ().scale(0.6f)*cUnitQuat::ZY().scale(0.15f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left palm"), cUnitQuat::ZY().scale(0.45f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right palm"), cUnitQuat::ZY().scale(0.45f));

	//Head tilt forward
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 2"), cUnitQuat::ZY().scale(0.2f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 3"), cUnitQuat::ZY().scale(0.3f));
	mNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 4"), cUnitQuat::ZY().scale(0.35f));
	//haunches shift back
	mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left femur"), cUnitQuat::ZY().scale(0.2f));
	mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left tibia"), cUnitQuat::YZ().scale(0.05f)*cUnitQuat::XZ().scale(0.05f));
	mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right femur"), cUnitQuat::ZY().scale(0.2f));
	mNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right tibia"), cUnitQuat::YZ().scale(0.05f)*cUnitQuat::ZX().scale(0.05f));
	mNodes.emplace_back(mParent.GetSkeletonBase(), cUnitQuat::YZ().scale(0.0f));
	
	mLeftHeel = mParent.GetSkeletonBase()->FindDescendant("left hindfoot");
	mRightHeel = mParent.GetSkeletonBase()->FindDescendant("right hindfoot");
	mSpineInitialPos = mParent.GetSpineEnd()->GetAccumPosition();
	mSpineTarget = cCoord3(0.0, 5.0, 0.5);
	Init();
}

void cRearUpAnimation::Init()
{
	if (IsPerturbing())
	{
		SetDuration(1.4f);
	}
	else{
		SetDuration(0.6f);
	}

	cSceneGraphNode* right_toes = mParent.GetSkeletonBase()->FindDescendant("right forefoot");
	mFootPosition = mRightHeel->GetAccumPosition();
	mFootDirection = (right_toes->GetAccumPosition()-mFootPosition).normalized();
}

void cRearUpAnimation::DoFrame()
{
	const float fraction = IsPerturbing() ? FractionComplete() : 1.0f - FractionComplete();
	const float scale = sqrtf(0.5f*(1.0f-cosf(1.0f*AKJ_PI*fraction)));

	for (auto i = mNodes.begin(); i != mNodes.end() ; ++i )
	{
		i->mNode->SetRotation(i->mTargetRotation.scale(scale));
	}
	mParent.CurlSpineTowards(cUnitQuat(mSpineInitialPos, mSpineTarget).scale(scale));
	const cCoord3 pos_heel = mRightHeel->GetAccumPosition();
	cCoord3 adjust = cCoord3(0.0f, -1.0f, -1.0f)*( pos_heel - mFootPosition);
	mParent.GetSkeletonBase()->AddTranslation(adjust);
	mRightHeel->SetRotationFromWorldVector(mFootDirection);
	mLeftHeel->SetRotationFromWorldVector(cCoord3(-1.0f, 1.0f, 1.0f )*mFootDirection);
	//printf("scale: %f \n",scale);
}


//////////////////////////////////////////////////////////////////////////
cJumpAnimation::cJumpAnimation( Rabbit& parent, eAnimationDirection direction, eAnimationType type )
	: mCrouchPhase(0.2f)
	, mLeapPhase(0.2f)
	, mDesiredDuration(1.8f)
	, cRabbitAnimation(parent, "might as well Jump!", direction, type)

{
	mCrouchNodes.clear();
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left shoulder"), cUnitQuat::XZ().scale(0.0f));
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right shoulder"), cUnitQuat::ZX().scale(0.0f));
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left elbow"), cUnitQuat::XZ().scale(0.0f)*cUnitQuat::ZY().scale(0.10f));
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right elbow"), cUnitQuat::ZX().scale(0.0f)*cUnitQuat::ZY().scale(0.10f));
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left wrist"), cUnitQuat::YZ().scale(0.34f)*cUnitQuat::XZ().scale(0.34f));
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right wrist"), cUnitQuat::YZ().scale(0.34f)*cUnitQuat::ZX().scale(0.34f));
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left palm"), cUnitQuat::ZX().scale(0.28f)*cUnitQuat::ZY().scale(0.28f));
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right palm"), cUnitQuat::XZ().scale(0.28f)*cUnitQuat::ZY().scale(0.28f));
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 2"),cUnitQuat(cCoord3::YAxis(), cCoord3::ZAxis()) );
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 3"),cUnitQuat::ZY().scale(0.0) );
	mCrouchNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 4"),cUnitQuat::ZY().scale(1.3) );
	mCrouchNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left femur"), cUnitQuat::ZY().scale(0.1f));
	mCrouchNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left tibia"), cUnitQuat::ZY().scale(0.10f)*cUnitQuat::XZ().scale(0.0f));
	mCrouchNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right femur"), cUnitQuat::ZY().scale(0.1f));
	mCrouchNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right tibia"), cUnitQuat::ZY().scale(0.10f)*cUnitQuat::ZX().scale(0.0f));

	mLeftHeel = mParent.GetSkeletonBase()->FindDescendant("left hindfoot");
	mRightHeel = mParent.GetSkeletonBase()->FindDescendant("right hindfoot");

	mLeftToe = mParent.GetSkeletonBase()->FindDescendant("left forefoot");
	mRightToe = mParent.GetSkeletonBase()->FindDescendant("right forefoot");


	mLeapNodes.clear();
	// arms hang to side
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left shoulder"), cUnitQuat::ZY().scale(0.58f)*cUnitQuat::ZX().scale(0.2f));
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right shoulder"), cUnitQuat::ZY().scale(0.58f)*cUnitQuat::XZ().scale(0.2f));
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left elbow"), cUnitQuat::YZ().scale(0.25f)*cUnitQuat::XZ().scale(0.05f));
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right elbow"), cUnitQuat::YZ().scale(0.25f)*cUnitQuat::ZX().scale(0.05f));
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left wrist"), cUnitQuat::ZX().scale(0.6f)*cUnitQuat::ZY().scale(0.15f));
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right wrist"), cUnitQuat::XZ().scale(0.6f)*cUnitQuat::ZY().scale(0.15f));
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("left palm"), cUnitQuat::ZY().scale(0.45f));
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("right palm"), cUnitQuat::ZY().scale(0.45f));
	//Head tilt forward
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 2"), cUnitQuat::ZY().scale(0.2f));
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 3"), cUnitQuat::ZY().scale(0.3f));
	mLeapNodes.emplace_back(mParent.GetSpineEnd()->FindDescendant("neck 4"), cUnitQuat::ZY().scale(0.35f));
	//haunches shift back
	mLeapNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left femur"), cUnitQuat::YZ().scale(0.4f));
	mLeapNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("left tibia"), cUnitQuat::ZY().scale(0.30f)*cUnitQuat::ZX().scale(0.15f));
	mLeapNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right femur"), cUnitQuat::YZ().scale(0.4f));
	mLeapNodes.emplace_back(mParent.GetSkeletonBase()->FindDescendant("right tibia"), cUnitQuat::ZY().scale(0.30f)*cUnitQuat::ZX().scale(0.15f));
	mSpineInitialPos = mParent.GetSpineEnd()->GetAccumPosition();
	mCrouchSpineTarget = cCoord3(0.0, 0.0, -4.0);
	mLeapSpineTarget = cCoord3(0.0, 5.0, 0.5);
	cSceneGraphNode* right_toes = mParent.GetSkeletonBase()->FindDescendant("right forefoot");
	mFootPosition = mRightHeel->GetAccumPosition();
	mToePosition = mRightToe->GetAccumPosition();
	mFootDirection = (right_toes->GetAccumPosition()-mFootPosition).normalized();

	Init();
}

void cJumpAnimation::Init()
{
	SetDuration(mDesiredDuration);
}

void cJumpAnimation::DoFrame()
{
	const float fraction = IsPerturbing() ? FractionComplete() : 1.0f - FractionComplete();

	if(fraction <= mCrouchPhase)
	{
		const float crouch_fraction =  fraction/mCrouchPhase;
		const float scale = sqrtf(0.5f*(1.0f-cosf(1.0f*AKJ_PI*crouch_fraction)));

		for (auto i = mCrouchNodes.begin(); i != mCrouchNodes.end() ; ++i )
		{
			i->mNode->SetRotation(i->mTargetRotation.scale(scale));
		}
		mParent.CurlSpineTowards(cUnitQuat(mSpineInitialPos, mCrouchSpineTarget).scale(scale));
		const cCoord3 pos_heel = mRightHeel->GetAccumPosition();
		cCoord3 adjust = cCoord3(0.0f, -1.0f, -1.0f)*( pos_heel - mFootPosition);
		mParent.GetSkeletonBase()->AddTranslation(adjust);
		mRightHeel->SetRotationFromWorldVector(mFootDirection);
		mLeftHeel->SetRotationFromWorldVector(cCoord3(-1.0f, 1.0f, 1.0f )*mFootDirection);
		mHipHeight = mParent.GetSkeletonBase()->GetTranslation();
	}
	else if (fraction <= mCrouchPhase+mLeapPhase)
	{

		LeapPhase(fraction);

	
	}
	else if (fraction > mCrouchPhase+mLeapPhase)
	{
		float air_fraction = 0.0f;
		if(!IsPerturbing())
		{
			air_fraction = 1.0f-0.5f*((fraction -(mCrouchPhase+mLeapPhase))/(1.0-(mCrouchPhase+mLeapPhase)));
		}
		else
		{
			air_fraction = 0.5f*((fraction -(mCrouchPhase+mLeapPhase))/(1.0-(mCrouchPhase+mLeapPhase)));
			// if we're in jump cut mode, should trigger the Leap phase transformations
			if(mType == cAnimation::JUMP)
			{
				LeapPhase(mCrouchPhase+mLeapPhase);
			}
		}
		const float total_air_time = 2.0f*mDesiredDuration*(1.0f-(mCrouchPhase+mLeapPhase));
		const float air_time = air_fraction*total_air_time;
		const float g = -3.9;
		const float vi = -g*total_air_time;
		const float height = air_time*(vi + g*air_time);
		//printf("height = %f %f \n", height, air_time);
		mParent.GetSkeletonBase()->SetTranslation(cCoord3(0.0f, height, 0.0f) + mHipHeight);
	}

	mLastFraction = fraction;
}

void cJumpAnimation::LeapPhase( const float fraction )
{
	const float leap_fraction =  (fraction - mCrouchPhase)/mLeapPhase;
	const float scale = (1.0f-cosf(0.5f*AKJ_PI*leap_fraction));
	for (size_t i = 0; i < mLeapNodes.size() ; ++i )
	{
		const cUnitQuat crouch_part = mCrouchNodes.at(i).mTargetRotation.scale(1.0f-scale);
		const cUnitQuat leap_part = mLeapNodes.at(i).mTargetRotation.scale(scale);
		mLeapNodes.at(i).mNode->SetRotation(crouch_part*leap_part);
	}
	const cUnitQuat crouch_spine = cUnitQuat(mSpineInitialPos, mCrouchSpineTarget).scale(1.0f-scale);
	const cUnitQuat leap_spine = cUnitQuat(mSpineInitialPos, mLeapSpineTarget).scale(scale);
	mParent.CurlSpineTowards(crouch_spine*leap_spine);
	cUnitQuat toe_quat = cUnitQuat::ZY().scale(scale*0.5);
	mRightHeel->SetRotationFromWorldVector(toe_quat.rotate(mFootDirection));
	mLeftHeel->SetRotationFromWorldVector(toe_quat.rotate(cCoord3(-1.0f, 1.0f, 1.0f )*mFootDirection));
	const cCoord3 pos_toe = mRightToe->GetAccumPosition();
	cCoord3 adjust = cCoord3(0.0f, -1.0f, -1.0f)*( pos_toe - mToePosition);
	mParent.GetSkeletonBase()->AddTranslation(adjust);
	mHipHeight = mParent.GetSkeletonBase()->GetTranslation();
}

} // end namespace akj

