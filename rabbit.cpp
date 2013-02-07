#include "rabbit.hpp"
#include "cs314_util.hpp"
#include "rabbit_animations.hpp"

namespace akj{

void Rabbit::CreateCurvedPoints(cCoord3* points,int num_points, const cCoord3& start, const cCoord3& end, const cCoord3& rotation_center, float factor /* = 1.0f*/)
{

	const cUnitQuat curvature(start - rotation_center, end-rotation_center);
	const cCoord3 end_rot_part = curvature.rotate(start-rotation_center) +rotation_center;
	const cCoord3 end_translate_part = end - end_rot_part;
	
	float weight = 1.0f;
	// we start at index one in this interp, not 0
	double sum = 1.0;
	for (int i = 0; i < num_points - 1; ++i)
	{
		sum += weight;
		weight = weight*factor;
	}
	const float scale = 1.0f/static_cast<float>(sum);
	weight = 1.0f;
	float interp = weight*scale;
	
	for (int i = 0; i < num_points; i++)
	{
		points[i] = interp*end_translate_part + rotation_center + curvature.scale(interp).rotate(start-rotation_center);
		interp += weight*scale;
		weight *= factor;
	}
}

void Rabbit::CreateLeftAndRightShells(tBoneNode* base, const char* name_1,  const char* name_2 ,
							  const cCoord3& start_adjust, const cCoord3& end_adjust,
							  const cCoord2& aspect, int num_strips, int num_sections)
{
	const cCoord3 left(-1.0f, 1.0f, 1.0f);
	std::string prefix("right ");
	cSceneGraphNode* right_bone_1 = base->FindDescendant((prefix+name_1).c_str());
	cSceneGraphNode* right_bone_2 = base->FindDescendant((prefix+name_2).c_str());
	assert(right_bone_1);
	cEllipseCubedNode* right_shell = right_bone_1->addChild<cEllipseCubedNode>((prefix + name_1 + std::string(" ellipse")).c_str()); 
	
	right_shell->CreateEllipsoidAlongZ(right_bone_2, start_adjust, end_adjust, aspect, num_strips, num_sections);
	prefix = "left ";
	cSceneGraphNode* left_bone_1 = base->FindDescendant((prefix+name_1).c_str());
	cSceneGraphNode* left_bone_2 = base->FindDescendant((prefix+name_2).c_str());
	assert(left_bone_1);
	cEllipseCubedNode* left_shell = left_bone_1->addChild<cEllipseCubedNode>((prefix + name_1 + std::string(" ellipse")).c_str()); 
	left_shell->CreateEllipsoidAlongZ(left_bone_2, start_adjust*left, end_adjust*left, aspect, num_strips, num_sections);
	mEllipseNodes.push_back(right_shell);
	mEllipseNodes.push_back(left_shell);
}

//static function
std::unique_ptr<tBoneNode> Rabbit::CreateSkeleton()
{
	const cCoord3 hip_to_ground(0.0f, -1.2f, 0.0f);
	const cCoord3 hip_to_shoulder(0.0, 1.6, -3.2);
	const cCoord3 shoulder_to_ground(0.0f, hip_to_ground.y-hip_to_shoulder.y,  0.0f);
	
	
	const int spine_sections = 10;
	const cCoord3 spine_rotation_center = cCoord3(0.0f, 0.1f*hip_to_shoulder.y, 0.40f*hip_to_shoulder.z);
	const float spine_base_thickness = 0.03f;
	const float spine_end_thickness = 0.014f;
	const float spine_scaling = 0.95f;

	const int neck_sections_1 = 5;
	const int neck_sections_2 = 2;
	const int neck_sections = neck_sections_1 + neck_sections_2;
	const cCoord3 shoulder_to_neck_end(0.0f, 0.56f, -0.7f);
	const float neck_inflection_point = 0.70f;
	const cCoord3 neck_rotation_center_1 = neck_inflection_point*0.70f*shoulder_to_neck_end -0.1f*shoulder_to_ground;
	const cCoord3 neck_rotation_center_2 = 1.1*neck_inflection_point*shoulder_to_neck_end+ 0.05f*shoulder_to_ground;
	const float neck_base_thickness = spine_end_thickness;
	const float neck_end_thickness = spine_end_thickness*1.4;
	const float neck_scaling = 1.1f;

	const cCoord3 skull_base(0.0f, 0.625f, -1.28f);
	const cCoord3 skull_slope(0.0f,-0.86f,-1.0f);
	
	const int tail_sections = 9;
	const cCoord3 hip_to_tail_end(0.0f, -0.0f, 0.25f);
	const cCoord3 tail_rotation_center = 0.6f * hip_to_tail_end - 0.1f * hip_to_ground;
	const float tail_base_thickness = spine_base_thickness;
	const float tail_end_thickness = 0.004f;
	const float tail_scaling = spine_scaling;


	
	tBoneNode* base = new tBoneNode ("Rabbit origin", NULL);
	base->SetShape(cCoord3(0.01));
	
	/////////////////////
	// BUILD THE LEGS!
	/////////////////////
	const int leg_sections = 6;
	const char* right_leg_names[] =
	{
		"right pelvis",
		"right femur",
		"right tibia",
		"right hindfoot",
		"right forefoot",
		"right leg end"
	};
	const char* left_leg_names[] =
	{
		"left pelvis",
		"left femur",
		"left tibia",
		"left hindfoot",
		"left forefoot",
		"left leg end"
	};
	cCoord3 leg_locations[] =
	{
		cCoord3(0.20f, 0.0f, 0.0f) +0.1*hip_to_ground, //hip
		cCoord3(0.5f, 0.0f, 0.0f) + 0.45*hip_to_shoulder, // knee
		cCoord3(0.3f, 0.0f, 0.16f*hip_to_shoulder.z) + hip_to_ground, // heel
		cCoord3(0.35f,0.0f, 0.53f*hip_to_shoulder.z) + 0.916f*hip_to_ground, // ball of foot
		cCoord3(0.4f, 0.0f, 0.625*hip_to_shoulder.z) + hip_to_ground,// end of toes
		cCoord3(0.4f, 0.0f, 0.625*hip_to_shoulder.z+0.0001f) + hip_to_ground// end of toes
	};
	float thicknesses[] =
	{
		0.030f, 0.02f, 0.02f, 0.016f, 0.014f, 0.0001f
	};


	/////////////////////
	// BUILD THE SPINE!
	/////////////////////

	const char* spine_names[spine_sections] =
	{
		"spine base",
		"spine 1",
		"spine 2",
		"spine 3",
		"spine 4",
		"spine 5",
		"spine 6",
		"spine 7",
		"spine 8",
		"spine end"
	};

	cCoord3 spine_locations[spine_sections];
	float spine_thicknesses[spine_sections];
	CreateCurvedPoints(spine_locations, spine_sections, cCoord3::Zeros(), hip_to_shoulder, spine_rotation_center, spine_scaling);
	GerpToArray(spine_thicknesses, spine_sections, spine_base_thickness, spine_end_thickness, spine_scaling);

	/////////////////////
	// BUILD THE TAIL!
	/////////////////////

	const char* tail_names[tail_sections] =
	{
		"tail base",
		"tail 1",
		"tail 2",
		"tail 3",
		"tail 4",
		"tail 5",
		"tail 6",
		"tail 7",
		"tail end"
	};
	cCoord3 tail_locations[tail_sections];
	float tail_thicknesses[tail_sections];
	CreateCurvedPoints(tail_locations, tail_sections, cCoord3::Zeros(), hip_to_tail_end, tail_rotation_center, tail_scaling);
	GerpToArray(tail_thicknesses, tail_sections, tail_base_thickness, tail_end_thickness, tail_scaling);

	/////////////////////
	// BUILD THE NECK!
	/////////////////////

	const char* neck_names[neck_sections] =
	{
		"neck base",
		"neck 1",
		"neck 2",
		"neck 3",
		"neck 4",
		"neck 5",
		"neck end"
	};
	cCoord3 neck_locations[neck_sections];
	float neck_thicknesses[neck_sections];
	CreateCurvedPoints(neck_locations, neck_sections_1, cCoord3::Zeros(),neck_inflection_point*shoulder_to_neck_end, neck_rotation_center_1);
	CreateCurvedPoints(neck_locations + neck_sections_1, neck_sections_2,neck_inflection_point*shoulder_to_neck_end, shoulder_to_neck_end, neck_rotation_center_2, neck_scaling);
	GerpToArray(neck_thicknesses, neck_sections, neck_base_thickness, neck_end_thickness, neck_scaling);

	/////////////////////
	// BUILD THE ARMS!
	/////////////////////

	const int arm_sections = 6;

	const char* right_arm_names[] =
	{
		"right shoulder",
		"right elbow",
		"right wrist",
		"right palm",
		"right fingers",
		"right arm end"
	};
	const char* left_arm_names[] =
	{
		"left shoulder",
		"left elbow",
		"left wrist",
		"left palm",
		"left fingers",
		"left arm end"
	};
	cCoord3 arm_locations[] =
	{
		cCoord3(0.3f, 0.0f, -0.33f) 	+ 0.2f*shoulder_to_ground, // shoulder
		cCoord3(0.48f, 0.0f, 0.85f) 		+ 0.41*shoulder_to_ground, // elbow
		cCoord3(0.28f, 0.0f, 0.4f) 		+ shoulder_to_ground, // wrist
		cCoord3(0.28f, 0.0f, 0.2f) 		+ shoulder_to_ground, // end of palm
		cCoord3(0.28f, 0.0f, -0.05f) 	+ shoulder_to_ground,// end of fingers
		cCoord3(0.28f, 0.0f, -0.0501f) 	+ shoulder_to_ground// end of fingers
	};
	float arm_thicknesses[] =
	{
		0.030f, 0.02f, 0.015f, 0.022f, 0.014f, 0.01f, 0.0001f
	};
	//CreateEllipsoidAlongZ(base, 0.2f*hip_to_ground-0.05*hip_to_shoulder, 0.2f*hip_to_ground+1.1f*hip_to_shoulder, cCoord2(0.6, 1.1), 22, 25);
	CreateBones(base,cCoord3::ZAxis(), leg_locations, thicknesses, right_leg_names, leg_sections);
	CreateBones(base,cCoord3::ZAxis(), leg_locations, thicknesses, left_leg_names, leg_sections, -1.0f);
	CreateBones(base,cCoord3::ZAxis(), tail_locations, tail_thicknesses, tail_names, tail_sections);
	tBoneNode* shoulder_base = CreateBones(base,cCoord3::ZAxis(),spine_locations, spine_thicknesses, spine_names, spine_sections);

	cCoord3 shoulder_orientation = shoulder_base->GetAccumOrientation().rotate(cCoord3::ZAxis());
	cCoord3 shoulder_accum = shoulder_base->GetAccumPosition();
	//printf("shoulder orientation: (%f, %f, %f)\n", shoulder_orientation.x, shoulder_orientation.y, shoulder_orientation.z);

	CreateBones(shoulder_base, shoulder_orientation, arm_locations, arm_thicknesses, right_arm_names, arm_sections);
	CreateBones(shoulder_base, shoulder_orientation, arm_locations, arm_thicknesses, left_arm_names, arm_sections, -1.0f);
	CreateBones(shoulder_base, shoulder_orientation, neck_locations, neck_thicknesses, neck_names,neck_sections);
	
	cEllipseCubedNode* body = base->addChild<cEllipseCubedNode>("main body ellipse");
	mEllipseNodes.push_back(body);
	cSceneGraphNode* neck = base->FindDescendant("neck 4");
	assert(neck);
	body->CreateEllipsoidAlongZ(neck, 0.2f*hip_to_ground-0.01f*hip_to_shoulder, 0.1f*hip_to_ground+0.03*hip_to_shoulder, cCoord2(0.6, 1.35), 75, 75);
	
	cEllipseCubedNode* tail_poof = base->addChild<cEllipseCubedNode>("tail ellipse"); 
	mEllipseNodes.push_back(tail_poof);
	cSceneGraphNode* tail = base->FindDescendant("tail end");
	assert(tail);
	//create tail
	tail_poof->CreateEllipsoidAlongZ(tail, 0.2f*hip_to_ground-0.07f*hip_to_shoulder, -0.4f*hip_to_ground-0.00f*hip_to_shoulder, cCoord2(0.4, 0.1), 27, 21);

	CreateLeftAndRightShells(base, "femur", "tibia", 0.2*hip_to_ground, 0.06*hip_to_shoulder, cCoord2(0.3, 0.6), 43, 33);
	CreateLeftAndRightShells(base, "tibia", "hindfoot", -0.05*hip_to_ground, 0.0*hip_to_shoulder, cCoord2(0.2, 0.3), 33, 23);
	CreateLeftAndRightShells(base, "hindfoot", "leg end", 0.0f*hip_to_ground, 0.04f*cCoord3::ZAxis(), cCoord2(0.13, 0.1), 21, 15);
	
	const cCoord3 upper_arm_length = (arm_locations[1]-arm_locations[0]);
	const cCoord3 forearm_length = (arm_locations[2]-arm_locations[1]);
	CreateLeftAndRightShells(shoulder_base, "elbow", "wrist", 0.0*hip_to_ground, 0.11*upper_arm_length, cCoord2(0.25, 0.22), 33, 21);
	CreateLeftAndRightShells(shoulder_base, "wrist", "palm", -0.1*forearm_length, 0.00*hip_to_shoulder, cCoord2(0.18, 0.2), 31, 15);
	CreateLeftAndRightShells(shoulder_base, "palm", "arm end", 0.0*hip_to_ground, cCoord3::Zeros(), cCoord2(0.06, 0.11), 31, 15);


	return std::unique_ptr<tBoneNode>(base);
}

tBoneNode* Rabbit::CreateHead( cSceneGraphNode* neck_end )
{
	tBoneNode* skull = neck_end->addChild<tBoneNode>("rabbit skull");
	skull->SetBaseOrientation(cUnitQuat::ZY().scale(0.6f));
	cCoord3 skull_z = skull->GetAccumOrientation().rotate(cCoord3::ZAxis());
	//TODO change this
	skull->SetShape(cCoord3(0.01f, 0.0f, 0.01f));
	//skull->SetTranslation(cCoord3(0.0, 1.0f, 0.4f));
	//skull->SetOrigin(cCoord3(-1.0f, -1.0f, 0.4f));
	tBoneNode* ear_left = skull->addChild<tBoneNode>("left ear");
	tBoneNode* ear_right = skull->addChild<tBoneNode>("right ear");
	cEllipseCubedNode* ear_left_cover = ear_left->addChild<cEllipseCubedNode>("left ear cover");
	cEllipseCubedNode* ear_right_cover = ear_right->addChild<cEllipseCubedNode>("right ear cover");
	cEllipseCubedNode* eye_left = skull->addChild<cEllipseCubedNode>("left eye");
	cEllipseCubedNode* eye_right = skull->addChild<cEllipseCubedNode>("right eye");
	cEllipseCubedNode* head_ellipse = skull->addChild<cEllipseCubedNode>("head ellipse");
	mEllipseNodes.push_back(ear_left_cover);
	mEllipseNodes.push_back(ear_right_cover);
	mEllipseNodes.push_back(eye_left);
	mEllipseNodes.push_back(eye_right);
	mEllipseNodes.push_back(head_ellipse);
	
	//skull->AddSimpleChild(cCubeDescriptor( cCoord3(-0.2f, -1.35f, 0.0f),cCoord3(2.2f, 1.6f, 0.1f), cUnitQuat()));
	head_ellipse->CreateEllipsoidAlongZ(NULL,-0.5f*cCoord3::ZAxis(), 1.1f*cCoord3::ZAxis(), cCoord2(0.42, 0.52), 45, 35 );
	const cCoord3 ear_shape(0.3f, -1.4f, -0.3f);
	const cCoord3 ear_start(0.20f, -0.25f, -0.1f);

	float eye_size = 0.3f;
	const cCoord2 eye_aspect(0.5f*eye_size, 0.5f*eye_size);
	const cCoord3 eye_start(0.33f, -0.24f, 0.3f);
	const cCoord3 left(-1.0f, 1.0f, 1.0f);
	ear_right->SetShape(cCoord3(0.0f));
	ear_left->SetShape(cCoord3(0.0f));
	ear_right->SetOrigin(ear_start);
	ear_right->SetTranslation(ear_start);
	ear_left->SetOrigin(ear_start*left);
	ear_left->SetTranslation(ear_start*left);
	ear_left_cover->CreateEllipsoidAlongZ(NULL, cCoord3::Zeros(), (ear_shape)*left, cCoord2(0.15f, 0.1f), 21, 17);
	ear_right_cover->CreateEllipsoidAlongZ(NULL, cCoord3::Zeros() , ear_shape, cCoord2(0.15f, 0.1f), 21, 17);
	eye_left->SetOrigin(left*eye_start);
	eye_left->SetTranslation(left*eye_start);
	eye_right->SetOrigin(eye_start);
	eye_right->SetTranslation(eye_start);
	eye_left->CreateEllipsoidAlongZ(NULL,  -eye_size*0.5f*cCoord3::ZAxis(), eye_size*0.5f*cCoord3::ZAxis(), eye_aspect, 15, 11);
	eye_right->CreateEllipsoidAlongZ(NULL, -eye_size*0.5f*cCoord3::ZAxis(), eye_size*0.5f*cCoord3::ZAxis(), eye_aspect, 15, 11);
	eye_left->SetBaseOrientation(cUnitQuat::ZX().scale(0.7f));
	eye_right->SetBaseOrientation(cUnitQuat::ZX().scale(-0.7f));

	return skull;
}

void Rabbit::Draw()
{
	glPushMatrix();
	glTranslatef(mTranslation.x, mTranslation.y, mTranslation.z);
	cAngleAxis aa = mRotation.to_angle_axis();
	if(aa.angle != 0.0f)
	{
		glRotatef(ToDegrees(aa.angle), aa.x, aa.y, aa.z);
	}
	glScalef(mScale.x, mScale.y, mScale.z);
	
	const cCoord3 com = GetCenterOfMass();
	glTranslatef(-com.x, -com.y, -com.z);
	mSkeleton->Draw();

	glPopMatrix();
}

bool Rabbit::Update( float elapsed_time )
{
	bool ret = false;
	for (size_t i = 0; i < mAnimations.size(); ++i)
	{
		if(mAnimations.at(i))
		{
			bool need_update = true;
			cRabbitAnimation* animation = mAnimations.at(i).get();
			if(!animation->IsComplete())
			{
				animation->Update(elapsed_time);
			}
			else if( cAnimation::CYCLE == animation->GetDirection())
			{
				SetAnimation(static_cast<eRabbitState>(i), cAnimation::RESTORE, animation->GetType());
			}
			else if( cAnimation::PERTURB_REPEAT == animation->GetDirection())
			{
				SetAnimation(static_cast<eRabbitState>(i), cAnimation::RESTORE_REPEAT, animation->GetType());
			}
			else if( cAnimation::RESTORE_REPEAT == animation->GetDirection())
			{
				SetAnimation(static_cast<eRabbitState>(i), cAnimation::PERTURB_REPEAT, animation->GetType());
			}
			else
			{
				need_update = false;
			}
			ret |= need_update;
		}
	}
	return ret;
}


tBoneNode* Rabbit::CreateBones( tBoneNode* parent,const cCoord3& orientation,const cCoord3* coords, float* thickness, const char** names, int num_bones, float lateral/* = 1.0f*/)
{
	const float thickness_scale = .0300f;
	cCoord3  old_direction = orientation;
	float length = parent->GetTranslation().length();
	cCoord3 origin = cCoord3::Zeros();
	cUnitQuat z_basis_quat( old_direction, cCoord3::ZAxis());

	for (int i = 0; i < num_bones; i++)
	{

		parent = parent->addChild<tBoneNode>(names[i]);
		cCoord3 dest = coords[i];
		dest.x *= lateral*1.3f;
		const cCoord3 new_direction = dest-origin;
		const float new_length = new_direction.length();
		
		
		parent->SetShape(cCoord3(thickness_scale*thickness[i], thickness_scale*thickness[i],  new_length));
		cUnitQuat rot(cCoord3::ZAxis(), z_basis_quat.rotate(new_direction));
		parent->SetBaseOrientation(rot);
		//parent->SetOrigin(0.5f*z_basis_quat.rotate(new_direction));
		parent->SetOrigin( 0.5*new_length*cCoord3::ZAxis() );
		parent->SetTranslation( length*cCoord3::ZAxis() );
		const cAngleAxis aa = rot.to_angle_axis();

		//printf("rotating: %f, around (%f,%f,%f)\n", ToDegrees(aa.angle), aa.x, aa.y, aa.z);

		z_basis_quat = z_basis_quat*cUnitQuat(new_direction, old_direction);
		length = new_length;
		old_direction = new_direction;
		origin = dest;
	}
	return parent;
}

void Rabbit::SetAnimation( eRabbitState anim_state, cAnimation::eAnimationDirection direction, cAnimation::eAnimationType mode )
{
	if(!mAnimations.at(anim_state))
	{
		switch (anim_state)
		{
		case akj::Rabbit::IDLE:
			break;
		case akj::Rabbit::HEAD_NOD:
			mAnimations.at(HEAD_NOD).reset(new cHeadNodAnimation(*this, direction, mode));
			break;
		case akj::Rabbit::LIMB_RAISE_RIGHT_HIND:
			mAnimations.at(anim_state).reset(new cHindLegRaiseAnimation(*this,false /*is right*/, direction, mode));
			break;
		case akj::Rabbit::LIMB_RAISE_LEFT_HIND:
			mAnimations.at(anim_state).reset(new cHindLegRaiseAnimation(*this,true /*is left*/, direction, mode));
			break;
		case akj::Rabbit::LIMB_RAISE_RIGHT_FORE:
			mAnimations.at(anim_state).reset(new cFrontLegRaiseAnimation(*this,false /*is right*/, direction, mode));
			break;
		case akj::Rabbit::LIMB_RAISE_LEFT_FORE:
			mAnimations.at(anim_state).reset(new cFrontLegRaiseAnimation(*this,true /*is left*/, direction, mode));
			break;
		case akj::Rabbit::EAR_RAISE_RIGHT:
			mAnimations.at(anim_state).reset(new cEarWiggleAnimation(*this,false /*is right*/, direction, mode));
			break;
		case akj::Rabbit::EAR_RAISE_LEFT:
			mAnimations.at(anim_state).reset(new cEarWiggleAnimation(*this,true /*is left*/, direction, mode));
			break;
		case akj::Rabbit::REAR_UP:
			mAnimations.at(anim_state).reset(new cRearUpAnimation(*this, direction, mode));
			break;
		case akj::Rabbit::BODY_CURL:
			mAnimations.at(anim_state).reset(new cBodyCurlAnimation(*this, direction, mode));
			break;
		case akj::Rabbit::JUMP:
			mAnimations.at(anim_state).reset(new cJumpAnimation(*this, direction, mode));
			break;
		case akj::Rabbit::ELLIPSETRANS:
			mAnimations.at(ELLIPSETRANS).reset(new cEllipseToCubeAnimation(*this, direction, mode));
			break;
		case akj::Rabbit::LAST_ANIM_ENTRY:
		default:
			break;
		}
	}
	else
	{
		mAnimations.at(anim_state)->Restart(direction, mode);
	}
}

void Rabbit::SetReverseAnimation( eRabbitState anim_state, cAnimation::eAnimationType mode )
{
	cAnimation::eAnimationDirection direction = cAnimation::PERTURB;
	if(mAnimations.at(anim_state))
	{
		cRabbitAnimation* animation = mAnimations.at(anim_state).get();
		if(animation->GetDirection() == cAnimation::PERTURB || animation->GetDirection() == cAnimation::CYCLE)
		{
			direction = cAnimation::RESTORE;
		}
		else if(animation->GetDirection() == cAnimation::PERTURB_REPEAT)
		{
			direction = cAnimation::RESTORE_REPEAT;
		}
		else if(animation->GetDirection() == cAnimation::RESTORE_REPEAT)
		{
			direction = cAnimation::PERTURB_REPEAT;
		}
	}
	SetAnimation(anim_state, direction, mode);

}

void Rabbit::CurlSpineTowards( const cUnitQuat& rot)
{
	// for the spine end:
	// find the initial orientation
	// find the initial position
	// then, get the middle of the spine base, the end initial, and the end desired and
	// use the average as the rotation center
	//cCoord3 end_init_pos = mSpineEnd->GetAccumPosition();
	//cUnitQuat end_init_rot = mSpineEnd->GetRotation()*mSpineEnd->GetAccumBaseRotation();

	cSceneGraphNode* spine_mid = mSpineEnd;
	cSceneGraphNode* spine_next = mSpineEnd;
	const float scale_constant = 1.0/10.0;
	const cUnitQuat scaled_rotation = rot.scale(scale_constant);
	do
	{
		spine_mid = spine_next;
		spine_mid->SetRotation(scaled_rotation);
		spine_next = spine_mid->GetParent();
	}while(spine_mid != mSpineBase);

}	



} // end namespace akj

