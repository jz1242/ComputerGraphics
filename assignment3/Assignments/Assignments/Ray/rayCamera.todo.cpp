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

Point3D target;
bool initialized = false;
void RayCamera::drawOpenGL( void )
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (!initialized) {
		target[0] = direction[0] + position[0];
		target[1] = direction[1] + position[1];
		target[2] = direction[2] + position[2];
		initialized = true;
	}
	gluLookAt(position[0], position[1], position[2], target[0], target[1], target[2], up[0], up[1], up[2]);
}
void RayCamera::rotateUp(Point3D center, float angle) {
	angle = -angle;
	Point3D k = up;
	position = position * cos(angle) + (Point3D::CrossProduct(k, position))*sin(angle) + k * (Point3D::Dot(k, position))*(1 - cos(angle));
	direction = (direction * cos(angle) + Point3D::CrossProduct(up, direction)*sin(angle) + up * Point3D::Dot(up, direction)*(1 - cos(angle)));
	right = (Point3D::CrossProduct(direction, up)).unit();
	direction = direction.unit();
	target = direction + position;
}
void RayCamera::rotateRight(Point3D center, float angle) {
	angle = -angle;
	Point3D k = right;
	position = position * cos(angle) + (Point3D::CrossProduct(k, position))*sin(angle) + k * (Point3D::Dot(k, position))*(1 - cos(angle));
	direction = (direction * cos(angle) + Point3D::CrossProduct(k, direction)*sin(angle) + k * Point3D::Dot(k, direction)*(1 - cos(angle)));
	up = (up * cos(angle) + Point3D::CrossProduct(k, up)*sin(angle) + k * Point3D::Dot(k, up)*(1 - cos(angle))).unit();
	direction = direction.unit();
	target = direction + position;

}
void RayCamera::moveForward( float dist )
{
	position += direction * dist;
	target = direction + position;
}
void RayCamera::moveRight( float dist )
{
	position += right * dist;
	target = direction + position;
}
void RayCamera::moveUp( float dist )
{
	position += up * dist;
	target = direction + position;
}

