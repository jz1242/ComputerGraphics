#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <math.h>
#include "rayCamera.h"
#include "rayShape.h"



//////////////////
// OpenGL stuff //
//////////////////
void RayCamera::drawOpenGL( void )
{
	throw RayException( "RayCamera::drawOpenGL undefined" );
}
void RayCamera::rotateUp( Point3D center , float angle )
{
	throw RayException( "RayCamera::rotateUp undefined" );
}
void RayCamera::rotateRight( Point3D center , float angle )
{
	throw RayException( "RayCamera::rotateRight undefined" );
}
void RayCamera::moveForward( float dist )
{
	throw RayException( "RayCamera::moveForward undefined" );
}
void RayCamera::moveRight( float dist )
{
	throw RayException( "RayCamera::moveRight undefined" );
}
void RayCamera::moveUp( float dist )
{
	throw RayException( "RayCamera::moveUp undefined" );
}
