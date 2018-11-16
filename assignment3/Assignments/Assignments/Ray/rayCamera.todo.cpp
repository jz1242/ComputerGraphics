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
	gluLookAt(position[0], position[1], position[2], direction[0] + position[0], direction[1] + position[1], direction[2] + position[2], up[0], up[1], up[2]);

}
void RayCamera::rotateUp( Point3D center , float angle )
{
	//glTranslatef(-center[0], -center[1], -center[2]);
	//glRotatef(angle, 0.0f, 1.0f, 0.0f);
	//glTranslatef(center[0], center[1], center[2]);
	//glFlush();
}
void RayCamera::rotateRight( Point3D center , float angle )
{
	//glTranslatef(-center[0], -center[1], -center[2]);
	//glRotatef(angle, 1.0f, 0.0f, 0.0f);
	//glTranslatef(center[0], center[1], center[2]);
	//glFlush();
}
void RayCamera::moveForward( float dist )
{
	position += direction * dist;
}
void RayCamera::moveRight( float dist )
{
	position += right * dist;
}
void RayCamera::moveUp( float dist )
{
	position += up * dist;
}

