#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rayTriangle.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
void RayTriangle::initialize(void)
{
	//static bool firstTime = true;
	//if (firstTime) fprintf(stderr, "[WARNING] RayTriangle::intialize undefined");
	//firstTime = false;
}
double RayTriangle::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	throw RayException("RayTriangle::intersect undefined");
	return -1;
}
BoundingBox3D RayTriangle::setBoundingBox(void)
{
	Point3D pList[3];
	for (int i = 0; i < 3; i++) pList[i] = v[i]->position;

	bBox = BoundingBox3D(pList, 3);
	for (int i = 0; i < 3; i++) bBox.p[0][i] -= RAYEPS, bBox.p[1][i] += RAYEPS;
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayTriangle::drawOpenGL(int materialIndex, GLSLProgram * glslProgram)
{
	glBegin(GL_TRIANGLES);
	glVertex3f(v[0]->position[0], v[0]->position[1], v[0]->position[2]);
	glNormal3f(v[0]->normal[0], v[0]->normal[1], v[0]->normal[2]);
	glVertex3f(v[1]->position[0], v[1]->position[1], v[1]->position[2]);
	glNormal3f(v[1]->normal[0], v[1]->normal[1], v[1]->normal[2]);
	glVertex3f(v[2]->position[0], v[2]->position[1], v[2]->position[2]);
	glNormal3f(v[2]->normal[0], v[2]->normal[1], v[2]->normal[2]);
	glEnd();
	glFlush();
	return -1;
}