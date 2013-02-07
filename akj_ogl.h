#ifndef AKJ_OGL_H
#define AKJ_OGL_H
#include <GL/glut.h>
#include <GL/glext.h>
#include <memory>
#include <vector>
#include <string>
//forward declarations
class cUnitQuat;
class cAngleAxis;



#ifdef WIN32
	extern PFNGLACTIVETEXTUREPROC glActiveTexture;
	extern PFNGLBLENDEQUATIONPROC glBlendEquation;
#endif

extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
extern PFNGLISSHADERPROC glIsShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLVALIDATEPROGRAMPROC glValidateProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM2FPROC glUniform2f;
extern PFNGLUNIFORM2FPROC glUniform3f;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;


namespace akj{

	extern const GLfloat kCubeVerts[];
	extern const GLfloat kCubeTexCoords[];
	extern const GLfloat kCubeNormals[]; 
	void glInit();
	const char* glErrorStringFromCode(int code);
	int glCheckAllErrors(const char* file, int line);
	void glDrawCube();
	void glDrawRect2D();
	const GLubyte* glTestTexture();
	GLuint glLoadTexture2D(const char* file_name, bool is_srgb = false, int mip_levels = 0);
	GLuint glCreateCubeMap(const char* base_name, bool is_srgb = false);
	void glRotateq(const cUnitQuat& quaternion);
	void ResetCubeDrawCount();
	int GetCubeDrawCount();


	class cGLObject
	{
	public:
		cGLObject(const char* object_name);
		virtual ~cGLObject(){};
		virtual GLuint GetID() const {return mObjectID;};
		virtual const char* GetName() const {return mObjectName;};
		virtual void Bind()=0;
	protected:
		GLuint mObjectID;
		const char* mObjectName;
	};

	class cGLTexture : public cGLObject
	{
	public:
		cGLTexture(const char* object_name);
		~cGLTexture();
		void CreateCubeMap( const char* base_name, bool is_srgb = false );
		void CreateTexture2D( const char* base_name, bool is_srgb = false, int miplevels = -1 );
		void CreateEmptyTexture2D( int width, int height, GLint internal_format );
		GLint GetBoundTextureUnit() const {return mBoundTextureUnit;}
		void Bind();

	private:
		GLint mBoundTextureUnit;
	};

	class cGLRenderBuffer : public cGLObject
	{
	public:
		cGLRenderBuffer(const char* object_name);
		~cGLRenderBuffer();
		void InitStorage(int width, int height, GLenum internal_format);

		//inherited
		void Bind();
	};

	class cGLFrameBuffer : public cGLObject
	{
	public:
		cGLFrameBuffer(const char* object_name);
		~cGLFrameBuffer();
		void Bind();
		void InitBuffers(int width, int height, GLint internal_format = GL_RGBA8, bool create_depth_buffer = true);
		void AttachTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
		GLint GetBoundTextureUnit() const;

		int mWidth;
		int mHeight;
	private:

		std::unique_ptr<cGLRenderBuffer> mDepthBuffer;
		std::unique_ptr<cGLTexture> mColorBuffer;
	};
	class cGLShader
	{
	public:
		struct cShaderFile
		{
			std::string mFileName;
			std::string mFileText;
			std::vector<int> mLineIndices;
		};
		cGLShader();
		~cGLShader();
		bool SetFragmentShader(const char** file_names, int num_files);
		bool SetVertexShader(const char** file_names, int num_files);
		bool LinkShaderProgram();
		bool Use();
		void BindUniformToInt(const char* uniform_name, int value);
		void BindUniformToFloat(const char* uniform_name, float value);
		GLint GetUniformLocation(const char * uniform_name);
		//const char* GetSamplerFileName(const char * uniform_name);
	private:
		std::vector<cShaderFile> mShaderFiles;
		bool IsShaderAttached(GLuint shader_id);
		GLint GetShaderType(GLuint shader_id);
		int LoadShaderImpl(GLuint shader_id, const char** file_names, int num_files);
		GLuint mShaderProgram;
		GLuint mFragmentShader;
		GLuint mVertexShader;
		const char* mVersionString;
	};
}
#endif
