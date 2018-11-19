#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayScene.h"
#include "rayCylinder.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCylinder::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	throw RayException("RayCylinder::intersect undefined");
	return -1;
}

BoundingBox3D RayCylinder::setBoundingBox(void)
{
	Point3D p(radius, height / 2, radius);
	bBox = BoundingBox3D(center + p, center - p);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
void RayCylinder::setUpOpenGL(int cplx, bool setBufferObjects)
{
	openGLComplexity = cplx;
	_vertexArrayID = _elementBufferID = _vertexBufferID = 0;
}

int RayCylinder::drawOpenGL(int materialIndex, GLSLProgram * glslProgram)
{
	if (materialIndex != material->index) {
		material->drawOpenGL(glslProgram);
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	int res = openGLComplexity;
	double p1 = 1;
	double p2 = 0;
	double angle = (2 * PI) / res;
	double yTop = center[1] + (height / 2);
	double yBot = center[1] - (height / 2);
	double xRad = center[0] + radius;
	double zRad = center[2] + radius;
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i <= res; i++) {
		double cosine = cos(angle * i);
		double cosine1 = cos(angle * (i + 1));
		double sine = sin(angle * i);
		double sine1 = sin(angle * (i + 1));

		glVertex3f(center[0], yTop, center[2]);
		glNormal3f(0, 1, 0);
		glVertex3f(xRad*cosine1, yTop, zRad*sine1);
		glNormal3f(0, 1, 0);
		glVertex3f(xRad*cosine, yTop, zRad*sine);
		glNormal3f(0, 1, 0);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= res; i++){
		double cosine = cos(angle * i);
		double cosine1 = cos(angle * (i + 1));
		double sine = sin(angle * i);
		double sine1 = sin(angle * (i + 1));
		Point3D n = { xRad*cosine, center[2], zRad*sine };
		n = n.unit();
		glVertex3f(xRad*cosine, yBot, zRad*sine);
		glNormal3f(n[0], n[1], n[2]);
		glVertex3f(xRad*cosine1, yTop, zRad*sine1);
		glNormal3f(n[0], n[1], n[2]);
	}
	glEnd();


	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i <= res; i++) {
		double cosine = cos(angle * i);
		double cosine1 = cos(angle * (i + 1));
		double sine = sin(angle * i);
		double sine1 = sin(angle * (i + 1));

		glVertex3f(center[0], yBot, center[2]);
		glNormal3f(0, -1, 0);
		glVertex3f(xRad*cosine, yBot, zRad*sine);
		glNormal3f(0, -1, 0);
		glVertex3f(xRad*cosine1, yBot, zRad*sine1);
		glNormal3f(0, -1, 0);
	}
	glEnd();

	glPopAttrib();
	return material->index;
}