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

	double angle = (PI / res) * 2;
	double p1 = 1;
	double p2 = 0;

	double half = center[1] + (height / 2);
	double halfbot = center[1] - (height / 2);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= res; i++) {
		double cosine = cos(angle * i);
		double cosine1 = cos(angle * (i + 1));
		double sine = sin(angle * i);
		double sine1 = sin(angle * (i + 1));
		glVertex3f(center[0], half, center[2]);
		glNormal3f(0, 1, 0);
		glVertex3f(radius * cosine1 + center[0], half, radius * sine1);
		glNormal3f(0, 1, 0);
		glVertex3f(radius * cosine + center[0], half, radius * sine + center[2]);
		glNormal3f(0, 1, 0);
	}
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= res; i++) {
		glVertex3f(center[0] + p1, halfbot, center[2] + p2);
		glNormal3f(p1, 0, p2);
		glVertex3f(center[0] + p1, half, center[2] + p2);
		glNormal3f(p1, 0, p2);
		//rotate point
		double a = cos(angle) * p1 - sin(angle) * p2;
		double b = sin(angle) * p1 + cos(angle) * p2;
		p1 = a;
		p2 = b;
	}
	glEnd();


	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i <= res; i++) {
		double cosine = cos(angle * i);
		double cosine1 = cos(angle * (i + 1));
		double sine = sin(angle * i);
		double sine1 = sin(angle * (i + 1));

		glVertex3f(center[0], halfbot, center[2]);
		glNormal3f(0, -1, 0);
		glVertex3f(radius * cosine + center[0], halfbot, radius * sine + center[2]);
		glNormal3f(0, -1, 0);
		glVertex3f(radius * cosine1 + center[0], halfbot, radius * sine1);
		glNormal3f(0, -1, 0);
	}
	glEnd();

	glPopAttrib();
	return material->index;
}