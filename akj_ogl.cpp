#include "akj_ogl.h"
#include "cs314_util.hpp"
#include "cs314_math.hpp"
#include <stdio.h>
#include <assert.h>
#ifndef STRINGIFY
#define STRINGIFY(var) #var
#endif

static const double kLogOf2 =  0.6931471805599453;
#ifdef WIN32
	#define AKJ_GET_GL_FUNCTION_ADDRESS(name,type) {name = (type)wglGetProcAddress(#name); assert(name != NULL);}
#else   //linux or mac
	#include "GL/glx.h"
	#define AKJ_GET_GL_FUNCTION_ADDRESS(name,type)	{name = (type)glXGetProcAddress((const GLubyte*)#name); assert(name != NULL);}
#endif 

#define PRINT_GL_INT_DEF(enum) {GLint i = 0; glGetIntegerv(enum, &i); printf("%5d = %s\n", i, #enum);}


#ifdef WIN32
	PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
	PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;
#endif
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = NULL;
PFNGLISSHADERPROC glIsShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLUNIFORM2FPROC glUniform2f = NULL;
PFNGLUNIFORM2FPROC glUniform3f = NULL;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = NULL;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = NULL;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = NULL;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = NULL;





namespace akj{
	static int glCubeDrawCalls = 0;
void ResetCubeDrawCount()
{
	glCubeDrawCalls = 0;
}
int GetCubeDrawCount()
{
	return glCubeDrawCalls;
}

//draws a cube by reading vertices from predefined static arrays
void glDrawCube()
{
	glCubeDrawCalls++;
	glNormalPointer(GL_FLOAT, 0, kCubeNormals);
	glVertexPointer(3, GL_FLOAT, 0, kCubeVerts);
	glTexCoordPointer(3, GL_FLOAT, 0, kCubeTexCoords);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void glDrawRect2D()
{
	glNormalPointer(GL_FLOAT, 0, kCubeNormals);
	glVertexPointer(3, GL_FLOAT, 0, kCubeVerts);
	glTexCoordPointer(3, GL_FLOAT, 0, kCubeTexCoords);
	glDrawArrays(GL_TRIANGLES, 12, 6);
}

//instead of using glew, which was continually causing me problems on windows
void glInit()
{
	AKJ_GET_GL_FUNCTION_ADDRESS(glCreateProgram, PFNGLCREATEPROGRAMPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glCreateShader, PFNGLCREATESHADERPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glCompileShader, PFNGLCOMPILESHADERPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glDeleteShader, PFNGLDELETESHADERPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glDeleteProgram, PFNGLDELETEPROGRAMPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glShaderSource, PFNGLSHADERSOURCEPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glGetShaderiv, PFNGLGETSHADERIVPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glAttachShader, PFNGLATTACHSHADERPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glGetAttachedShaders, PFNGLGETATTACHEDSHADERSPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glIsShader, PFNGLISSHADERPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glLinkProgram, PFNGLLINKPROGRAMPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glGetProgramiv, PFNGLGETPROGRAMIVPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glValidateProgram, PFNGLVALIDATEPROGRAMPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glUseProgram, PFNGLUSEPROGRAMPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glGetAttribLocation, PFNGLGETATTRIBLOCATIONPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glCreateProgram, PFNGLCREATEPROGRAMPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glUniform1i, PFNGLUNIFORM1IPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glUniform2f, PFNGLUNIFORM2FPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glUniform3f, PFNGLUNIFORM2FPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glDeleteFramebuffers, PFNGLDELETEFRAMEBUFFERSPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glFramebufferTexture2D, PFNGLFRAMEBUFFERTEXTURE2DPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glCheckFramebufferStatus, PFNGLCHECKFRAMEBUFFERSTATUSPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glDeleteRenderbuffers, PFNGLDELETERENDERBUFFERSPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glFramebufferRenderbuffer, PFNGLFRAMEBUFFERRENDERBUFFERPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glUniform1f, PFNGLUNIFORM1FPROC);
	AKJ_GET_GL_FUNCTION_ADDRESS(glBlendFuncSeparate, PFNGLBLENDFUNCSEPARATEPROC);


	#ifdef WIN32
		AKJ_GET_GL_FUNCTION_ADDRESS(glBlendEquation, PFNGLBLENDEQUATIONPROC);
		AKJ_GET_GL_FUNCTION_ADDRESS(glActiveTexture, PFNGLACTIVETEXTUREPROC);
	#endif // WIN32

	PRINT_GL_INT_DEF(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
	PRINT_GL_INT_DEF(GL_MAX_VERTEX_ATTRIBS);
	PRINT_GL_INT_DEF(GL_MAX_TEXTURE_COORDS);
	PRINT_GL_INT_DEF(GL_MAX_TEXTURE_IMAGE_UNITS);
	PRINT_GL_INT_DEF(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);
	PRINT_GL_INT_DEF(GL_MAX_VERTEX_UNIFORM_COMPONENTS);
	PRINT_GL_INT_DEF(GL_MAX_VARYING_FLOATS);
	PRINT_GL_INT_DEF(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
	PRINT_GL_INT_DEF(GL_MAX_CUBE_MAP_TEXTURE_SIZE);
}

const char* glErrorStringFromCode( int code )
{
	const char* ret = "";
	switch (code)
	{
	case GL_NO_ERROR:
		ret = "GL_NO_ERROR";
		break;
	case GL_INVALID_ENUM:
		ret = "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		ret = "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		ret = "GL_INVALID_OPERATION";
		break;
	case GL_STACK_OVERFLOW:
		ret = "GL_STACK_OVERFLOW";
		break;
	case GL_STACK_UNDERFLOW:
		ret = "GL_STACK_UNDERFLOW";
		break;
	case GL_OUT_OF_MEMORY:
		ret = "GL_OUT_OF_MEMORY";
		break;
	case GL_TABLE_TOO_LARGE:
		ret = "TABLE_TOO_LARGE";
		break;
	default:
		ret = "Unknown error";
	}
	return ret;
}

int glCheckAllErrors( const char* file, int line )
{
	int error_count = 0;
	for (GLint error_code = glGetError(); error_code != GL_NO_ERROR; error_code = glGetError())
	{
		if(0 == error_count)
		{
			printf("found glError(s) at %s: %d\n", file, line);
		}
		++error_count;

		printf("## %3d >> glError %x : %s\n", error_count, error_code, glErrorStringFromCode(error_code));
	}
	if(error_count > 0)
	{
		printf("## end errors\n");
	}
	return error_count;
}

int glCheckFrameBuffer(GLenum target)
{
	GLenum result = glCheckFramebufferStatus(target);
	if(GL_FRAMEBUFFER_COMPLETE == result)
	{
		return 0;
	}
	switch (result)
	{
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("framebuffer error: %s\n","GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		printf("framebuffer error: %s\n","GL_FRAMEBUFFER_UNDEFINED");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("framebuffer error: %s\n","GL_FRAMEBUFFER_UNSUPPORTED");
		break;
	case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER:
		printf("framebuffer error: %s\n","GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("framebuffer error: %s\n","GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf("framebuffer error: %s\n","GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		printf("framebuffer error: %s\n","GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
		break;
	default:
		printf("unrecognized error: 0x%x\n", result);
		break;
	}
	return 1;
}

void glRotateq(const cUnitQuat& quaternion)
{
	cAngleAxis aa = quaternion.to_angle_axis();
	if(aa.angle != 0.0f)
	{
		glRotatef(ToDegrees(aa.angle), aa.x, aa.y, aa.z);
	}
}

const GLubyte* glTestTexture()
{
	static const GLubyte kTestImage[64] = 
	{
		0,   128,   124, 0,
		 255,   0,   0, 255,
		   0, 255,   0, 255,
		   0,   0, 255, 0,
		 255, 255,   0, 0,
		   0, 255, 255, 255,
		 255,   0, 255, 0,
		 255, 255,   0, 255,
		 255,   0, 255, 255,
		   0, 255,   0, 0,
		   0,   0, 255, 255,
		   0, 128,   0, 255,
		   0, 128,   0, 255,
		   0,   0, 128, 255,
		   0,   0, 128, 255,
		   0,   0, 128, 255,
	};
	return kTestImage;
}

cGLObject::cGLObject( const char* object_name ):mObjectName(object_name),mObjectID(GL_INVALID_VALUE)
{
}


cGLShader::cGLShader():mShaderProgram(0),mFragmentShader(0),mVertexShader(0),mVersionString("#version 120\n")
{
	mShaderProgram = glCreateProgram();
	mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	mVertexShader = glCreateShader(GL_VERTEX_SHADER);
	if(glCheckAllErrors(__FILE__, __LINE__))
	{
		printf("there were errors constructing the shader object.\n");
	}
}

cGLShader::~cGLShader()
{
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mFragmentShader);
	glDeleteShader(mVertexShader);
}

bool cGLShader::SetFragmentShader( const char** file_names, int num_files )
{
	if(GL_FRAGMENT_SHADER != GetShaderType(mFragmentShader))
	{
		printf("can't set fragment shader: mFragmentShader is not a valid shader object\n");
		return false;
	}
	if(GL_NO_ERROR != LoadShaderImpl(mFragmentShader, file_names, num_files))
	{
		return false;
	}
	return true;
}

bool cGLShader::SetVertexShader( const char** file_names, int num_files  )
{
	if(GL_VERTEX_SHADER != GetShaderType(mVertexShader))
	{
		printf("can't set fragment shader: mVertexShader is not a valid shader object\n");
		return false;
	}
	if(GL_NO_ERROR != LoadShaderImpl(mVertexShader, file_names, num_files))
	{
		return false;
	}
	return true;
}

int cGLShader::LoadShaderImpl( GLuint shader_id, const char** file_names, int num_files )
{

	std::vector<const char*> string_pointers;
	int start_index = static_cast<int>(mShaderFiles.size());

	string_pointers.push_back(mVersionString);

	//load the files
	for (int i = 0; i < num_files; i++)
	{
		mShaderFiles.push_back(cShaderFile());
		mShaderFiles.back().mFileName = file_names[i];
		if(FileToString(mShaderFiles.back().mFileName.c_str(), mShaderFiles.back().mFileText) != 0)
		{
			return 1;
		}
	}
	for (int i = start_index; i < start_index+num_files; i++)
	{
		string_pointers.push_back(mShaderFiles.at(i).mFileText.c_str());
	}
	glShaderSource(shader_id, static_cast<int>(string_pointers.size()), &string_pointers.at(0), NULL);
	
	if(glCheckAllErrors(__FILE__, __LINE__))
	{
		printf("there were errors setting shader source for file '%s' (ID 0x%x).\n",  file_names[num_files-1], shader_id );
		return 1;
	}
	if(GL_FALSE == glIsShader(shader_id))
	{
		printf("somehow this is not a shader object now (id = %x). \n", shader_id );
		return 1;
	}

	glCompileShader(shader_id);
	
	if(glCheckAllErrors(__FILE__, __LINE__))
	{
		printf("there were errors compiling shader %x, file: %s.\n", shader_id, file_names[num_files-1] );
		return 1;
	}

	const char* compile_status_string = "Failed";
	GLint compiled_ok = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);
	if(compiled_ok == GL_TRUE)
	{
		compile_status_string = "Succeeded";
	}
	printf("%s: Compilation of shader %s.\n", compile_status_string, file_names[num_files-1]);
	GLint log_length = 0; 
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
	std::string info_log;
	if(log_length > 1)
	{
		GLsizei actual_length = 0;
		info_log.resize(log_length, ' ');
		glGetShaderInfoLog(shader_id, log_length, &actual_length, &info_log[0]);
		info_log.resize(actual_length);
		printf("log:\n%s\n", info_log.c_str());
	}
	
	if(glCheckAllErrors(__FILE__, __LINE__) || GL_FALSE == compiled_ok)
	{
		printf("there were errors compiling shader %x, file: %s.\n", shader_id, file_names[num_files-1] );
		return 1;
	}

	if(!IsShaderAttached(shader_id))
	{
		glAttachShader(mShaderProgram, shader_id);
	}

	if(glCheckAllErrors(__FILE__, __LINE__))
	{
		printf("there were errors attaching shader to program for shader %x, file: %s.\n", shader_id, file_names[num_files-1] );
		return 1;
	}
	
	//everything went better than expected :)
	return GL_NO_ERROR;
}

bool cGLShader::IsShaderAttached( GLuint shader_id )
{
	const GLsizei max_shaders = 4;
	GLsizei actual_shaders = 0;
	GLuint shaders[max_shaders];
	for (int i = 0 ; i < max_shaders; ++i)
	{
		shaders[i] = 0;
	}
	glGetAttachedShaders(mShaderProgram, max_shaders, &actual_shaders, shaders);
	for (int i = 0; i < actual_shaders; ++i)
	{
		if(shader_id == shaders[i])
		{
			return true;
		}
	}
	return false;
}

GLint cGLShader::GetShaderType( GLuint shader_id )
{
	GLint shader_type = 0;
	glGetShaderiv(shader_id, GL_SHADER_TYPE, &shader_type);
	if(glCheckAllErrors(__FILE__, __LINE__))
	{
		printf("there were errors checking the shader type for shader %x\n", shader_id);
	}
	return shader_type;
}

bool cGLShader::LinkShaderProgram()
{
	glLinkProgram(mShaderProgram);
	if(glCheckAllErrors(__FILE__, __LINE__))
	{
		printf("there were errors linking the shader program for program %x\n", mShaderProgram);
	}
	const char* link_status_string = "Failed";
	GLint linked_ok = GL_FALSE;
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &linked_ok);
	if(linked_ok == GL_TRUE)
	{
		link_status_string = "Succeeded";
	}
	printf("%s: Linking of shader.\n", link_status_string);

	GLint log_length = 0; 
	glGetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH, &log_length);
	std::string info_log;
	if(log_length > 1)
	{
		GLsizei actual_length = 0;
		info_log.resize(log_length, ' ');
		glGetProgramInfoLog(mShaderProgram, log_length, &actual_length, &info_log[0]);
		info_log.resize(actual_length);
		printf("log:\n%s\n", info_log.c_str());
	}

	return linked_ok == GL_TRUE;
}

bool cGLShader::Use()
{
	GLint is_linked = GL_FALSE;
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &is_linked);
	if(is_linked != GL_TRUE && !LinkShaderProgram())
	{
		return false;
	}
	glUseProgram(mShaderProgram);

	if(glCheckAllErrors(__FILE__, __LINE__))
	{
		printf("there were errors when trying to use the shader program\n");
		return false;
	}
	return true;
}

GLint cGLShader::GetUniformLocation( const char * uniform_name )
{
	return glGetUniformLocation(mShaderProgram, uniform_name);
}

void cGLShader::BindUniformToInt( const char* uniform_name, int value )
{
	Use();
	GLint uniform_location = GetUniformLocation(uniform_name);
	glUniform1i(uniform_location, value);
	glCheckAllErrors(__FILE__,__LINE__);
}

void cGLShader::BindUniformToFloat( const char* uniform_name, float value )
{
	Use();
	GLint uniform_location = GetUniformLocation(uniform_name);
	glUniform1f(uniform_location, value);
	glCheckAllErrors(__FILE__,__LINE__);
}



const GLfloat  kCubeVerts[108] =
{
	 0.5f, -0.5f, -0.5f, //0
	 0.5f, -0.5f,  0.5f, //1
	 0.5f,  0.5f, -0.5f, //2
	 0.5f,  0.5f, -0.5f, //2
	 0.5f, -0.5f,  0.5f, //1
	 0.5f,  0.5f,  0.5f, //3

	 0.5f,  0.5f, -0.5f, //2
	 0.5f,  0.5f,  0.5f, //3
	-0.5f,  0.5f, -0.5f, //6
	-0.5f,  0.5f, -0.5f, //6
	 0.5f,  0.5f,  0.5f, //3
	-0.5f,  0.5f,  0.5f, //7

	 0.5f, -0.5f,  0.5f, //1
	-0.5f, -0.5f,  0.5f, //5
	 0.5f,  0.5f,  0.5f, //3
	 0.5f,  0.5f,  0.5f, //3
	-0.5f, -0.5f,  0.5f, //5
	-0.5f,  0.5f,  0.5f, //7

	 0.5f, -0.5f, -0.5f, //0
	-0.5f, -0.5f, -0.5f, //4
	 0.5f, -0.5f,  0.5f, //1
	 0.5f, -0.5f,  0.5f, //1
	-0.5f, -0.5f, -0.5f, //4
	-0.5f, -0.5f,  0.5f, //5

	 0.5f,  0.5f, -0.5f, //2
	-0.5f,  0.5f, -0.5f, //6
	 0.5f, -0.5f, -0.5f, //0
	 0.5f, -0.5f, -0.5f, //0
	-0.5f,  0.5f, -0.5f, //6
	-0.5f, -0.5f, -0.5f, //4
	
	-0.5f, -0.5f, -0.5f, //4
	-0.5f, -0.5f,  0.5f, //5
	-0.5f,  0.5f, -0.5f, //6
	-0.5f,  0.5f, -0.5f, //6
	-0.5f, -0.5f,  0.5f, //5
	-0.5f,  0.5f,  0.5f  //7
};

const GLfloat kCubeNormals[108] = 
{
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,

	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,

	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,

	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,

	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,

	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f
};

const GLfloat kCubeTexCoords[108] = 
{
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,

	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,

	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,

	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,

	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,

	0.0f,  0.0f,  -1.0f,
	0.0f,  0.0f,  -1.0f,
	0.0f,  0.0f,  -1.0f,
	0.0f,  0.0f,  -1.0f,
	0.0f,  0.0f,  -1.0f,
	0.0f,  0.0f,  -1.0f
};

void cGLTexture::CreateTexture2D( const char* file_name, bool is_srgb /*= false*/, int mip_levels /*= -1*/ )
{
	cImageData image(file_name);
	if(NULL != image.Data())
	{
		GLint tex_unit_index;
		glGetIntegerv(GL_ACTIVE_TEXTURE, &tex_unit_index);
		tex_unit_index -= GL_TEXTURE0;
		glBindTexture(GL_TEXTURE_2D, mObjectID);
		if(glCheckAllErrors(__FILE__,__LINE__) > 0)
		{
			return;
		}
		mBoundTextureUnit = tex_unit_index;
		printf("loading image %s ", file_name);
		int bpp = image.BytesPerPixel();
		GLint internal_format = bpp;
		GLint my_format = bpp == 4 ? GL_RGBA : GL_RGB;
		if(is_srgb)
		{
			internal_format = bpp == 4 ? GL_SRGB8_ALPHA8 : GL_SRGB8;
		}
		else
		{
			internal_format = bpp == 4 ? GL_RGBA8 : GL_RGB8;
		}

		if(mip_levels < 0)
		{
			const int largest_side = image.Width() > image.Height() ? image.Width() : image.Height();
			mip_levels = static_cast<int>(floor(log(largest_side)/kLogOf2));
		}
		if(mip_levels > 0)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mip_levels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, 
			image.Width(), image.Height(), 0,
			my_format, GL_UNSIGNED_BYTE, image.Data());
		if(glCheckAllErrors(__FILE__, __LINE__))
		{
			printf("there were errors when initializing new 2D texture %s\n", image.FileName());
		}
		else
		{
			for (int i = 1; i <= mip_levels; i++)
			{
				image.GenerateMipData(i);
				glTexImage2D(GL_TEXTURE_2D, i, internal_format, 
					image.MipWidth(), image.MipHeight(), 0,
					my_format, GL_UNSIGNED_BYTE, image.MipData());
				printf(".");
			}
			if(glCheckAllErrors(__FILE__, __LINE__))
			{
				printf("there were errors when initializing new 2D texture %s\n", image.FileName());
			}
			else
			{
				printf(" done\n");
			}
		}
		glActiveTexture(GL_TEXTURE0 + tex_unit_index + 1);
	}
}

void cGLTexture::CreateCubeMap( const char* base_name, bool is_srgb /*= false*/ )
{
	static const char* prefixes[6] = {
		"posx_",
		"negx_",
		"posy_",
		"negy_",
		"posz_",
		"negz_"
	}; 
	static const GLint locations[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	GLint tex_unit_index;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &tex_unit_index);
	tex_unit_index -= GL_TEXTURE0;
	glBindTexture(GL_TEXTURE_CUBE_MAP, mObjectID);
	if(glCheckAllErrors(__FILE__,__LINE__) > 0)
	{
		return;
	}
	mBoundTextureUnit = tex_unit_index;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for (int i = 0; i < 6; i++)
	{
		std::string file_name(prefixes[i]);
		file_name.append(base_name);
		cImageData image(file_name.c_str());
		printf("loading image %s ", file_name.c_str());
		int bpp = image.BytesPerPixel();
		GLint internal_format = bpp;
		GLint my_format = bpp == 4 ? GL_RGBA : GL_RGB;
		if(is_srgb)
		{
			internal_format = bpp == 4 ? GL_SRGB8_ALPHA8 : GL_SRGB8;
		}
		else
		{
			internal_format = bpp == 4 ? GL_RGBA8 : GL_RGB8;
		}
		const int largest_side = image.Width() > image.Height() ? image.Width() : image.Height();
		const int mip_levels = static_cast<int>(floor(log(largest_side)/kLogOf2));
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mip_levels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexImage2D(locations[i], 0, internal_format, 
			image.Width(), image.Height(), 0,
			my_format, GL_UNSIGNED_BYTE, image.Data());
		if(glCheckAllErrors(__FILE__, __LINE__))
		{
			printf("there were errors when initializing new 2D texture %s\n", image.FileName());
		}
		else
		{
			for (int mip = 1; mip <= mip_levels; mip++)
			{
				image.GenerateMipData(mip);
				glTexImage2D(locations[i], mip, internal_format, 
					image.MipWidth(), image.MipHeight(), 0,
					my_format, GL_UNSIGNED_BYTE, image.MipData());
				printf(".");
			}
			if(glCheckAllErrors(__FILE__, __LINE__))
			{
				printf("there were errors when initializing new 2D texture %s\n", image.FileName());
			}
			else
			{
				printf(" done\n");
			} 
		}
	}
	glActiveTexture(GL_TEXTURE0 + tex_unit_index + 1);
}


cGLFrameBuffer::cGLFrameBuffer(const char* object_name):cGLObject(object_name)
{
	glGenFramebuffers(1, &mObjectID);
	glCheckAllErrors(__FILE__, __LINE__);
}

cGLFrameBuffer::~cGLFrameBuffer()
{
	glDeleteFramebuffers(1, &mObjectID);
}

void cGLFrameBuffer::AttachTexture2D( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
{
	glBindFramebuffer(target, mObjectID);
	glFramebufferTexture2D(target, attachment, textarget, texture, level);
	glCheckAllErrors(__FILE__, __LINE__);
	glCheckFrameBuffer(target);
}

void cGLFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mObjectID);
}

void cGLFrameBuffer::InitBuffers( int width, int height, GLint internal_format /*= GL_RGBA8*/, bool create_depth_buffer /*= true*/ )
{
	mWidth = width;
	mHeight = height;
	
	int current_tex_unit = 0; 
	glGetIntegerv(GL_ACTIVE_TEXTURE, &current_tex_unit);


	int color_tex_unit = mColorBuffer ? mColorBuffer->GetBoundTextureUnit() : -1;
	if(color_tex_unit != -1)
	{
		glActiveTexture(GL_TEXTURE0 + color_tex_unit);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, mObjectID);
	glCheckAllErrors(__FILE__, __LINE__);

	mColorBuffer.reset(new akj::cGLTexture("FBO color buffer attachment"));
	mColorBuffer->CreateEmptyTexture2D(width, height, internal_format);
	if(color_tex_unit != -1)
	{
		glActiveTexture(current_tex_unit + 1);
	}
	glCheckAllErrors(__FILE__, __LINE__);
	
	mDepthBuffer.reset(new akj::cGLRenderBuffer("FBO depth buffer attachment"));
	mDepthBuffer->InitStorage(width, height, GL_DEPTH_COMPONENT24);
	glCheckAllErrors(__FILE__, __LINE__);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer->GetID(), 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer->GetID() );
	glCheckAllErrors(__FILE__, __LINE__);
	glCheckFrameBuffer(GL_FRAMEBUFFER);

}

GLint cGLFrameBuffer::GetBoundTextureUnit() const
{
	if(mColorBuffer)
	{
		return mColorBuffer->GetBoundTextureUnit();
	}
	return -1;
}


cGLTexture::cGLTexture(const char* object_name):cGLObject(object_name),mBoundTextureUnit(-1)
{
	glGenTextures(1, &mObjectID);
	glCheckAllErrors(__FILE__, __LINE__);
}

cGLTexture::~cGLTexture()
{
	glDeleteTextures(1, &mObjectID);
}

void cGLTexture::CreateEmptyTexture2D( int width, int height, GLint internal_format )
{
	GLint tex_unit_index;
	GLint currently_bound = GL_INVALID_VALUE;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currently_bound);
	glGetIntegerv(GL_ACTIVE_TEXTURE, &tex_unit_index);
	tex_unit_index -= GL_TEXTURE0;
	glBindTexture(GL_TEXTURE_2D, mObjectID);
	if(glCheckAllErrors(__FILE__,__LINE__) > 0)
	{
		return;
	}
	mBoundTextureUnit = tex_unit_index;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	if(glCheckAllErrors(__FILE__, __LINE__))
	{
		printf("there were errors when initializing an empty 2D texture\n");
	}

	glActiveTexture(GL_TEXTURE0 + tex_unit_index + 1);
}

void cGLTexture::Bind()
{
	if(mBoundTextureUnit != -1)
	{
		return;
	}
}


cGLRenderBuffer::cGLRenderBuffer(const char* object_name):cGLObject(object_name)
{
	glGenRenderbuffers(1, &mObjectID);
	glCheckAllErrors(__FILE__,__LINE__);
}

cGLRenderBuffer::~cGLRenderBuffer()
{
	glDeleteRenderbuffers(1, &mObjectID);
}

void cGLRenderBuffer::InitStorage( int width, int height, GLenum internal_format )
{
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
	glCheckAllErrors(__FILE__,__LINE__);
}

void cGLRenderBuffer::Bind()
{
	glBindRenderbuffer(	GL_RENDERBUFFER , mObjectID);
}



}//end namespace akj
