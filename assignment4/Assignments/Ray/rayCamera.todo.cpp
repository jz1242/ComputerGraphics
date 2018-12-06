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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position[0], position[1], position[2], position[0] + direction[0], position[1] + direction[1], position[2] + direction[2], up[0], up[1], up[2]);
}
void RayCamera::rotateUp( Point3D center , float angle )
{
	//Util::Throw( "undefined" );
}
void RayCamera::rotateRight( Point3D center , float angle )
{
	//Util::Throw( "undefined" );
}
void RayCamera::moveForward( float dist )
{
	//Util::Throw( "undefined" );
}
void RayCamera::moveRight( float dist )
{
	//Util::Throw( "undefined" );
}
void RayCamera::moveUp( float dist )
{
	//Util::Throw( "undefined" );
}
