#define _CRT_SECURE_NO_WARNINGS	1
#include "akj_ogl.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include "cs314_math.hpp"
#include "cs314_util.hpp"
#include "scene_graph.hpp"
#include "rabbit.hpp"





int dumpPPM();
void keyboardCallback(unsigned char c, int x, int y);
void TriggerRabbitAnimation(akj::Rabbit::eRabbitState animation_state);
void mouseButtonCallback(int button, int state, int x, int y);
void mouseMotionCallback(int x, int y);
void reshapeCallback(int w, int h);
void displayCallback();
void animate( int last_frame );
int LoadTextures();



enum eUI_STATE
{
	UI_DEFAULT,
	UI_ROTATE,
	UI_ZOOM,
	UI_LOOK,
	UI_TRANSLATE
};

bool gDoSmoothAnimation = true;  // smooth or "jump cut"
bool gDoHalfAnimations = true;   // press buttton to cycle animation? or do half of it at a time?
eUI_STATE gUIState = UI_DEFAULT; // used for the mouse interaction state
float gRotate = 0.0f;
float gPitch = 0.0f;
float gTime = 0.0f; //float time sent to the shaders
float gMouseRate = 3.0f;  // how quickly does the onscreen action move with the mouse
cCoord3 gBaseTrans(1.0f,1.0f,1.0f);
int axis_choice = 0;
const int kFrameMS = 1000/30;
const float kRotationRate = 0.4f; // revolutions per second
cUnitQuat gClickOrientation;
cUnitQuat gRotationPerSecond;
cCoord2 gMousePos;
bool gMousePosValid;
unsigned char camera = 'r';
tBoneNode* gRightKnee = NULL;
akj::Rabbit* gRabbit = NULL;
cSceneGraphNode* gPostFXCube = NULL;
akj::cGLShader* gTexCubeShader = NULL;
akj::cGLShader* gBackgroundShader = NULL;
akj::cGLShader* gCompositingShader = NULL;
std::vector<std::unique_ptr<akj::cGLTexture> > gTextures;
akj::cGLFrameBuffer* gPostFXFrameBuffer = NULL;
bool gUseFBO = false;
bool gClearFBO = false;
float gFOV = 60.0f;
float gFarPlane = 20.0f;

int gImageCount = 0;       // used for numbering the PPM image files
int gWidth = 800;      // window width (pixels)
int gHeight = 600;     // window height (pixels)
bool gDump = false;      // flag set to true when dumping animation frames

//---------------------------------------------------------------

int dumpPPM()
{
	static int frameNum = 0;
	FILE *fp;
	const int maxVal = 255;
	register int y;
	unsigned char *pixels;

	glReadBuffer( GL_FRONT );
	char fname[100];
	sprintf(fname, "./ppm/img%03d.ppm", frameNum);
	fp = fopen(fname, "wb");
	if (!fp)
	{
		printf("Unable to open file '%s'\n", fname);
		return 1;
	}
	printf("Saving image `%s`\n", fname);
	fprintf(fp, "P6 ");
	fprintf(fp, "%d %d ", gWidth, gHeight);
	fprintf(fp, "%d", maxVal);
	putc(13, fp);
	pixels = new unsigned char [3 * gWidth];

	y = 0;
	glReadPixels(0, gHeight - 1, gWidth, 1, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *) pixels);

	for ( y = gHeight - 1; y >= 0; y-- )
	{
		glReadPixels(0, y, gWidth, 1, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *) pixels);
		for (int n = 0; n < 3 * gWidth; n++)
		{
			putc(pixels[n], fp);
		}
	}
	fclose(fp);
	delete [] pixels;
	frameNum++;
	return 0;
}
void keyboardCallback(unsigned char c, int x, int y)
{
	const int num_axes = 6;
	static float rotation_axes[6][3] =
	{
		{0.0, 0.0, 1.0},
		{0.0, 1.0, 0.0},
		{1.0, 0.0, 0.0},
		{1.0, 1.0, 0.0},
		{1.0, 0.0, 1.0},
		{0.0, 1.0, 1.0}
	};
	switch (c)
	{
	case 'm':
		TriggerRabbitAnimation(akj::Rabbit::LIMB_RAISE_RIGHT_FORE);
		break;
	case 'l':
		TriggerRabbitAnimation(akj::Rabbit::LIMB_RAISE_LEFT_FORE);
		break;
	case 'o':
		TriggerRabbitAnimation(akj::Rabbit::LIMB_RAISE_RIGHT_HIND);
		break;
	case 'n':
		TriggerRabbitAnimation(akj::Rabbit::LIMB_RAISE_LEFT_HIND);
		break;
	case 'e':
		TriggerRabbitAnimation(akj::Rabbit::EAR_RAISE_RIGHT);
		break;
	case 'w':
		TriggerRabbitAnimation(akj::Rabbit::EAR_RAISE_LEFT);
		break;
	case 'c':
		TriggerRabbitAnimation(akj::Rabbit::BODY_CURL);
		break;
	case 't':
		TriggerRabbitAnimation(akj::Rabbit::REAR_UP);
		break;
	case 'j':
		TriggerRabbitAnimation(akj::Rabbit::JUMP);
		break;
	case 'h':
		TriggerRabbitAnimation(akj::Rabbit::HEAD_NOD);
		break;
	case '2':
		TriggerRabbitAnimation(akj::Rabbit::ELLIPSETRANS);
		break;
	case 'f':
		gFOV += 1.0f;
		break;
	case 'v':
		gFOV -= 1.0f;
		break;
	case 'g':
		gFarPlane += 1.0f;
		break;
	case 'b':
		gFarPlane -= 1.0f;
		break;
	case 'q':
		exit (0);
		break;
	case '1':
		gUseFBO = !gUseFBO;
		gClearFBO = gUseFBO;
		break;
	case 'r':
		gRotationPerSecond = cUnitQuat();
		gRabbit->SetRotation(cUnitQuat());
		gRabbit->SetTranslation(cCoord3(0.0f));
		break;
	case 's':
		gRotationPerSecond = cUnitQuat();
		break;
	case 'i':
		dumpPPM();
		break;
	case 'd':               // dump animation PPM frames
		gImageCount = 0;         // image file count
		gDump = !gDump;
		break;
	case '-':
		gRabbit->MultiplyScale(cCoord3(2.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f));
		break;
	case '=':
		gRabbit->MultiplyScale(cCoord3(1.5f, 1.5f, 1.5f));
		break;
	case '3':
		gDoHalfAnimations = ! gDoHalfAnimations;
		break;
	case ' ':
		gDoSmoothAnimation = ! gDoSmoothAnimation;
		break;
	case 'x':
		gRotationPerSecond = cUnitQuat(cAngleAxis(kRotationRate * 2.0f * AKJ_PIf, rotation_axes[axis_choice][0], rotation_axes[axis_choice][1], rotation_axes[axis_choice][2]));
		axis_choice = (axis_choice + 1) % num_axes;
		break;
	}

	glutPostRedisplay();
}
void TriggerRabbitAnimation(akj::Rabbit::eRabbitState animation_state)
{
	const cAnimation::eAnimationType anim_mode = gDoSmoothAnimation ? cAnimation::SMOOTH : cAnimation::JUMP;
	if(gDoHalfAnimations)
	{
		gRabbit->SetReverseAnimation(animation_state, anim_mode);
	}
	else
	{
		gRabbit->SetAnimation(animation_state, cAnimation::CYCLE, anim_mode);
	}
}
void mouseButtonCallback(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
	
		if(state == GLUT_UP)
		{
			gUIState = UI_DEFAULT;
			gMousePosValid = false;
		}
		else if(state == GLUT_DOWN)
		{
			gUIState = UI_ROTATE;
			gMousePosValid = true;
			gMousePos = cCoord2(2.0f*x/gWidth - 1.0f, -(2.0f*(y)/gHeight - 1.0f));
			gClickOrientation = gRabbit->GetRotation();
		}
	break;
	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_UP)
		{
			gUIState = UI_DEFAULT;
			gMousePosValid = false;
		}
		else if(state == GLUT_DOWN)
		{
			gUIState = UI_LOOK;
			gMousePosValid = true;
			gMousePos = cCoord2(2.0f*x/gWidth - 1.0f, -(2.0f*(y)/gHeight - 1.0f));
			gClickOrientation = gRabbit->GetRotation();
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if(state == GLUT_UP)
		{
			gUIState = UI_DEFAULT;
			gMousePosValid = false;
		}
		else if(state == GLUT_DOWN)
		{
			gUIState = UI_ZOOM;
			gMousePosValid = true;
			gMousePos = cCoord2(2.0f*x/gWidth - 1.0f, -(2.0f*(y)/gHeight - 1.0f));
			gBaseTrans = gRabbit->GetTranslation();
		}
		break;
	default:
		break;
	}
}

void mouseMotionCallback(int x, int y)
{
	if(gUIState == UI_ROTATE && gMousePosValid)
	{
		const cCoord2 new_pos(2.0f*x/gWidth - 1.0f, -(2.0f*(y)/gHeight - 1.0f));
		const cCoord3 from = gRabbit->GetRotation().rev_rotate(cCoord3(gMouseRate*(gMousePos.x), gMouseRate*(gMousePos.y), 1.0f));
		const cCoord3 to = gRabbit->GetRotation().rev_rotate(cCoord3(gMouseRate*(new_pos.x), gMouseRate*(new_pos.y), 1.0f));
		gRabbit->AddRotation(cUnitQuat(from, to));
		gMousePos = new_pos;
	}
	else if(gUIState == UI_ZOOM && gMousePosValid)
	{
		const cCoord2 new_pos(2.0f*x/gWidth - 1.0f, -(2.0f*(y)/gHeight - 1.0f));
		float y_diff =  gMousePos.y - new_pos.y;
		gRabbit->SetTranslation((y_diff)*cCoord3::ZAxis()+gBaseTrans);
	}
	else if(gUIState == UI_LOOK && gMousePosValid)
	{
		const cCoord2 new_pos(2.0f*x/gWidth - 1.0f, -(2.0f*(y)/gHeight - 1.0f));
		const cCoord3 from = cCoord3(gMouseRate*(gMousePos.x-new_pos.x), gMouseRate*(gMousePos.y-new_pos.y), 1.0f);
		const cCoord3 to = cCoord3(0.0, 0.0, 1.0f);
		gPitch += 70.0f*(gMousePos.y-new_pos.y);
		gRotate += 120.0f*(new_pos.x - gMousePos.x);
		gMousePos = new_pos;
	}
}
void reshapeCallback(int w, int h)
{
   gWidth = w;
   gHeight = h;
   glViewport(0, 0, w, h);
   //have to resize our FBO at this point too
 

}

void SetupCamera() 
{
	// set up camera
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	double aspect = static_cast<double>(gWidth) / gHeight;
	gluPerspective( gFOV, aspect, 0.1, gFarPlane );
	glRotatef(gPitch, 1.0f, 0.0f, 0.0f);
	glRotatef(gRotate, 0.0f, 1.0f, 0.0f);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void RenderRabbit() 
{
	glPushMatrix();
	glLoadIdentity();
	glRotatef(-gRotate, 0.0f, 1.0f, 0.0f);
	glRotatef(-gPitch, 1.0f, 0.0f, 0.0f);
	glTranslatef( 0.0, 0.0, -3.0 );
	glPolygonMode(GL_FRONT, GL_FILL);
	gTexCubeShader->Use();
	gTexCubeShader->BindUniformToFloat("my_vertex_time", gTime);
	gRabbit->Draw();
	glPopMatrix();
	akj::glCheckAllErrors(__FILE__,__LINE__);
	
}

void RenderBackground() 
{
	glPushMatrix();
	gBackgroundShader->Use();
	glLoadIdentity();
	glScalef(15.0f, 15.0f, 15.0f);
	glPolygonMode(GL_BACK, GL_FILL);
	akj::glDrawCube();
	glPopMatrix();
	akj::glCheckAllErrors(__FILE__,__LINE__);
	
}

void displayCallback()
{
	akj::ResetCubeDrawCount();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	if(gUseFBO)
	{
		if(gWidth != gPostFXFrameBuffer->mWidth || gHeight != gPostFXFrameBuffer->mHeight)
		{
			gPostFXFrameBuffer->InitBuffers(gWidth, gHeight, GL_SRGB8_ALPHA8);
			//rebind to the normal back buffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFuncSeparate (GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
		RenderBackground();
		glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		//flame effects need to know about the time 
		gCompositingShader->BindUniformToFloat("my_time", gTime);

		glPolygonMode(GL_FRONT, GL_FILL);
		gCompositingShader->Use();
		// clear the color buffer
		// set up camera: we want a quad matched up to the viewport
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		//drawcube gives a -0.5 - 0.5 range, so mult by 2
		glScalef(2.0, 2.0, 1.0);
		glTranslatef( 0.0, 0.0, -0.8);
		akj::glDrawRect2D();
		akj::glCheckAllErrors(__FILE__,__LINE__);
		glClear( GL_DEPTH_BUFFER_BIT );
	}
	else
	{
		RenderBackground();
	}
	glClear( GL_DEPTH_BUFFER_BIT );
	glDisable(GL_BLEND);
	SetupCamera();
	RenderRabbit();

	if(gUseFBO)
	{
		if(gClearFBO)
		{
			glClear(  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			gClearFBO = false;
		}
		glActiveTexture(GL_TEXTURE0 + gPostFXFrameBuffer->GetBoundTextureUnit());
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, gWidth, gHeight);

	}

	glutSwapBuffers();
	if (gDump) {               // save images to file
		dumpPPM();
	}
	//printf("number of cubes this frame = %d\n", akj::GetCubeDrawCount());
	
}


void animate( int last_frame ) 
{

	const int current_time = glutGet(GLUT_ELAPSED_TIME);
	const int elapsed_time = current_time - last_frame;
	if(elapsed_time > 0){
		const float seconds_elapsed = (elapsed_time)*0.001f;
		//printf("elapsed: %f, total: %f\n", seconds_elapsed, gTime);
		gTime += seconds_elapsed;
		const cUnitQuat this_rot = gRotationPerSecond.scale(seconds_elapsed);
		cSceneGraphNode* node = NULL;
		gPostFXCube->AddRotation(this_rot);
		gRabbit->AddRotation(this_rot);
		gRabbit->Update(seconds_elapsed);

		glutPostRedisplay();
	}
	// Schedule the next frame.
	int waiting_time = kFrameMS - elapsed_time;
	if(waiting_time < 0)
	{
		waiting_time = 0;
	}
	glutTimerFunc(waiting_time, animate, current_time);
}

int LoadTextures()
{

	int error_count = 0;

	GLint topology_uniform = gTexCubeShader->GetUniformLocation("topology_sampler");
	const char* topology_filename = "gray50.tga";
	if(topology_uniform >= 0)
	{
		gTextures.emplace_back(new akj::cGLTexture("topology texture"));
		gTextures.back()->CreateTexture2D(topology_filename, false/*is_srgb*/, -1);
		gTexCubeShader->Use();
		glUniform1i(topology_uniform, gTextures.back()->GetBoundTextureUnit());
		error_count += akj::glCheckAllErrors(__FILE__, __LINE__);
	}

	GLint diffuse_uniform = gTexCubeShader->GetUniformLocation("diffuse_sampler");
	const char* diffuse_filename = "coldcolors.tga";
	if(diffuse_uniform >= 0)
	{
		gTextures.emplace_back(new akj::cGLTexture("diffuse color texture"));
		gTextures.back()->CreateTexture2D(diffuse_filename, true/*is_srgb*/, -1);
		gTexCubeShader->Use();
		glUniform1i(diffuse_uniform, gTextures.back()->GetBoundTextureUnit());
		error_count += akj::glCheckAllErrors(__FILE__, __LINE__);
	}
	
	GLint light_params_uniform = gTexCubeShader->GetUniformLocation("light_params_sampler");
	const char* light_params_filename = "gray50.tga";
	if(light_params_uniform >= 0)
	{
		gTextures.emplace_back(new akj::cGLTexture("light mapping texture"));
		gTextures.back()->CreateTexture2D(light_params_filename, false/*is_srgb*/, -1);
		gTexCubeShader->Use();
		glUniform1i(light_params_uniform, gTextures.back()->GetBoundTextureUnit());
		error_count += akj::glCheckAllErrors(__FILE__, __LINE__);
	}

	GLint texcube_refl_cube_uniform = gTexCubeShader->GetUniformLocation("detailed_cubemap");
	GLint background_refl_cube_uniform = gBackgroundShader->GetUniformLocation("my_enviro_map");

	const char* detailed_cube_filename = "cube_map.jpg";
	if(texcube_refl_cube_uniform >= 0 || background_refl_cube_uniform >= 0)
	{
		gTextures.emplace_back(new akj::cGLTexture("detailed cube texture"));
		gTextures.back()->CreateCubeMap(detailed_cube_filename, true /*is_srgb*/);
		gTexCubeShader->Use();
		glUniform1i(texcube_refl_cube_uniform, gTextures.back()->GetBoundTextureUnit());
		gBackgroundShader->Use();
		glUniform1i(background_refl_cube_uniform, gTextures.back()->GetBoundTextureUnit());
		error_count += akj::glCheckAllErrors(__FILE__, __LINE__);
	}

	GLint diffuse_cube_uniform = gTexCubeShader->GetUniformLocation("diffuse_cubemap");
	const char* diffuse_cube_filename = "diffuse_map.jpg";
	if(diffuse_cube_uniform >= 0)
	{
		gTextures.emplace_back(new akj::cGLTexture("diffuse cube texture"));
		//want the ambient light to be lighter, hence no srgb
		gTextures.back()->CreateCubeMap(diffuse_cube_filename, false/*is_srgb*/);
		gTexCubeShader->Use();
		glUniform1i(diffuse_cube_uniform, gTextures.back()->GetBoundTextureUnit());
		error_count += akj::glCheckAllErrors(__FILE__, __LINE__);
	}
	
	return error_count;
}
//---------------------------------------------------------------

int main(int argc, char **argv)
{

	gRotationPerSecond = cUnitQuat();
	gMousePosValid = false;

	// create window and rendering context
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA );
	glutInitWindowSize( gWidth, gHeight );
	glutCreateWindow( "Rambo The Rabbit" );
	// register glut callbacks
	glutDisplayFunc( displayCallback );
	glutMotionFunc( mouseMotionCallback );
	glutMouseFunc( mouseButtonCallback );
	glutKeyboardFunc( keyboardCallback );
	glutReshapeFunc( reshapeCallback );

	// extended gl function init: have to be loaded dynamically,
	// and we should only call this after we have a display context
	akj::glInit();

	// 
	glClearColor(0.0f, 0.1f, 0.3f, 0.0f);
	glViewport( 0, 0, gWidth, gHeight );
	glEnable( GL_FRAMEBUFFER_SRGB );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_NORMALIZE );
	glEnable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable (GL_BLEND);

	// can create some advanced objects, now that glInit has been called
	akj::cGLShader texcube_shader_actual;
	akj::cGLShader background_shader_actual;
	akj::cGLShader compositing_shader_actual;
	akj::Rabbit rabbit_actual("Rambuncio the rabbit");
	tBoneNode post_fx_cube_actual("post FX cube", NULL);
	akj::cGLFrameBuffer post_fx_fb_actual("post FX frame buffer");

	//initialize all our global pointers
	gPostFXFrameBuffer = &post_fx_fb_actual;
	gRabbit = &rabbit_actual;
	gPostFXCube = &post_fx_cube_actual;
	gTexCubeShader = &texcube_shader_actual;
	gCompositingShader = &compositing_shader_actual;
	gBackgroundShader = &background_shader_actual;

	gRabbit->SetScale(0.5f);
	gRabbit->SetRotation(cUnitQuat(cCoord3::ZAxis(), cCoord3::XAxis()));


	// load and compile all our shaders
	const char* texcube_fragment_shader[] = {
		"./cube_common.glsl",
		"./p1_fragment_shader.glsl"
	};
	const char* texcube_vertex_shader[] = {
		"./cube_common.glsl",
		"./noise4D.glsl",
		"./p1_vertex_shader.glsl"
	};
	gTexCubeShader->SetFragmentShader(texcube_fragment_shader, 2);
	gTexCubeShader->SetVertexShader(texcube_vertex_shader, 3);
	gTexCubeShader->LinkShaderProgram();

	const char* background_fragment_shader[] = {
		"./background_ps.glsl"
	};
	const char* background_vertex_shader[] = {
		"./background_vs.glsl"
	};
	gBackgroundShader->SetFragmentShader(background_fragment_shader, 1);
	gBackgroundShader->SetVertexShader(background_vertex_shader, 1);
	gBackgroundShader->LinkShaderProgram();

	const char* compositing_fragment_shader[] = {
		"./noise4D.glsl",
		"./compositing_ps.glsl"
	};
	const char* compositing_vertex_shader[] = {
		"./compositing_vs.glsl"
	};
	gCompositingShader->SetFragmentShader(compositing_fragment_shader, 2);
	gCompositingShader->SetVertexShader(compositing_vertex_shader, 1);
	gCompositingShader->LinkShaderProgram();
	
	//initalize the framebuffer to use for HDR + postFx
	gPostFXFrameBuffer->InitBuffers(gWidth, gHeight, GL_SRGB8_ALPHA8);
	gCompositingShader->BindUniformToInt("fbo_texture", gPostFXFrameBuffer->GetBoundTextureUnit());


	//load all the textures from disk
	LoadTextures();

	// Schedule the first animation callback 0 ms from the current time.
	const int current_time = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(0, animate, current_time);

	// pass control over to GLUT
	glutMainLoop();
  
	return 0;       // never reached
}
