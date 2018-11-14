//Code from : OpenGL 4 Shading Language Cookbook Second Edition, by David Wolff
// [MK] Modified to only support simple vertex/fragment shaders
#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <GL/glew.h>
#include <string>
#include <stdexcept>

#pragma warning( disable : 4290 )

/** This class describes the exception thrown when the GLSL fails to create/use the program correctly */
class GLSLProgramException : public std::runtime_error
{
public:
	GLSLProgramException( const std::string & msg ) : std::runtime_error( msg ) { }
};

namespace GLSLShaderInfo
{
	struct shader_file_extension
	{
		const char *ext;
		GLenum type;
	};

	const shader_file_extension extensions[] =
	{
		{ ".vs"   , GL_VERTEX_SHADER },
		{ ".vert" , GL_VERTEX_SHADER },
		{ ".fs"   , GL_FRAGMENT_SHADER },
		{ ".frag" , GL_FRAGMENT_SHADER },
	};
}

/** This class describes a GLSL program consisting of a vertex and fragment shader.
*/
class GLSLProgram
{
	template< unsigned int Dim > static void glUniformiv      ( GLint location , GLsizei count ,                       const GLint*   value );
	template< unsigned int Dim > static void glUniformfv      ( GLint location , GLsizei count ,                       const GLfloat* value );
	template< unsigned int Dim > static void glUniformMatrixfv( GLint location , GLsizei count , GLboolean transpose , const GLfloat* value );

	// Make these private in order to make the object non-copyable
	GLSLProgram( const GLSLProgram & other ) { }
	GLSLProgram & operator=( const GLSLProgram &other ){ return *this; }

	GLuint _handle;
	bool _linked;
	std::string _vertexSource;
	std::string _fragmentSource;
public:
	static bool FileExists( const std::string & fileName );
	static std::string GetExtension( const std::string & fileName );
	static GLenum GetShaderType( const char* fileName ) throw( GLSLProgramException );
	static const char * GetShaderTypeString( GLenum type );

	GLSLProgram( const char* vs_filename , const char* fs_filename ) throw( GLSLProgramException );
	~GLSLProgram( void );

	/** This method compiles the specified shader from the source */
	void compileShader( const std::string & source , GLenum type ) throw( GLSLProgramException );

	/** This method links together the vertex and fragment shaders */
	void link( void )     throw( GLSLProgramException );
	/** This method validates that the program was generated successfully */
	void validate( void ) throw( GLSLProgramException );
	/** This method enable the program */
	void use( void )      throw( GLSLProgramException );

	/** This method return the OpenGL handle to the shader */
	GLuint handle( void ) const { return _handle; }
	/** This method return a boolean value indicating if the shader was (successfully) linked */
	bool linked( void ) const { return _linked; }

	/** This method sets an integer uniform value with the specified name. */
	void setUniform( const char *name , int    val , bool showWarning=true );
	/** This method sets a single-precision floating point unfiform value with the specified name. */
	void setUniform( const char *name , float  val , bool showWarning=true );
	/** This method sets an double-precision floating uniform value with the specified name. */
	void setUniform( const char *name , double val , bool showWarning=true );
	/** This method sets an an integer array uniform value with the specified name. The template parameter indicates the dimensionality of the array. */
	template< unsigned int > void setUniform( const char* name , const int*    v , bool showWarning=true );
	/** This method sets an an single-precision floating point array uniform value with the specified name. The template parameter indicates the dimensionality of the array. */
	template< unsigned int > void setUniform( const char* name , const float*  v , bool showWarning=true );
	/** This method sets an an double-precision floating point array uniform value with the specified name. The template parameter indicates the dimensionality of the array. */
	template< unsigned int > void setUniform( const char* name , const double* v , bool showWarning=true );
	/** This method sets an a single-precision floating point matrix uniform value with the specified name. The template parameter indicates the dimensionality of the matrix. */
	template< unsigned int > void setUniformMatrix( const char* name , const float*  m , bool showWarning=true );
	/** This method sets an a double-precision floating point matrix uniform value with the specified name. The template parameter indicates the dimensionality of the matrix. */
	template< unsigned int > void setUniformMatrix( const char* name , const double* m , bool showWarning=true );

	/** This method sets up the program. */
	void setup( void );
};

template< unsigned int Dim >
void GLSLProgram::setUniform( const char* name , const int* v , bool showWarning )
{
	GLint loc = glGetUniformLocation( _handle , name );
	if( loc>=0 ) glUniformiv< Dim >( loc , 1 , v );
	else if( showWarning ) fprintf( stderr , "[WARNING] Non-existant uniform: %s\n" , name );
}
template< unsigned int Dim >
void GLSLProgram::setUniform( const char* name , const float* v , bool showWarning )
{
	GLint loc = glGetUniformLocation( _handle , name );
	if( loc>=0 ) glUniformfv< Dim >( loc , 1 , v );
	else if( showWarning ) fprintf( stderr , "[WARNING] Non-existant uniform: %s\n" , name );
}
template< unsigned int Dim >
void GLSLProgram::setUniform( const char* name , const double* v , bool showWarning )
{
	float _v[Dim];
	for( int i=0 ; i<Dim ; i++ ) _v[i] = (float)v[i];
	setUniform< Dim >( name , _v , showWarning );
}
template< unsigned int Dim >
void GLSLProgram::setUniformMatrix( const char* name , const float* m , bool showWarning )
{
	GLint loc = glGetUniformLocation( _handle , name );
	if( loc>=0 ) glUniformMatrixfv< Dim >( loc , 1 , GL_FALSE , m );
	else if( showWarning ) fprintf( stderr , "[WARNING] Non-existant uniform: %s\n" , name );
};
template< unsigned int Dim >
void GLSLProgram::setUniformMatrix( const char* name , const double* m , bool showWarning )
{
	float _m[Dim*Dim];
	for( int i=0 ; i<Dim*Dim ; i++ ) _m[i] = (float)m[i];
	setUniformMatrix< Dim >( name , _m , showWarning );
}

#endif // GLSLPROGRAM_H