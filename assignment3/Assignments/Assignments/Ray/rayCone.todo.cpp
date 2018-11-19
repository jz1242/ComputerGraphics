#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayScene.h"
#include "rayCone.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCone::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	throw RayException("RayCone::intersect undefined");
	return -1;
}

BoundingBox3D RayCone::setBoundingBox(void)
{
	Point3D p(radius, height / 2, radius);
	bBox = BoundingBox3D(center + p, center - p);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
void RayCone::setUpOpenGL(int cplx, bool setBufferObjects)
{
	openGLComplexity = cplx;
	_vertexArrayID = _elementBufferID = _vertexBufferID = 0;
	//throw RayException("RayCone::setUpOpenGL undefined");
}

int RayCone::drawOpenGL(int materialIndex, GLSLProgram * glslProgram)
{
	if (materialIndex != material->index) {
		material->drawOpenGL(glslProgram);
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	int res = openGLComplexity;
	double angle = (2 * PI) / res;
	double yTop = center[1] + (height / 2);
	double yBot = center[1] - (height / 2);
	double xRad = center[0] + radius;
	double zRad = center[2] + radius;
	double hyp = sqrt(pow(height, 2) + pow(radius, 2));
	glBegin(GL_TRIANGLE_STRIP);

	for (int i = 0; i <= res; i++) {
		double cosine = cos(angle * i);
		double cosine1 = cos(angle * (i + 1));
		double sine = sin(angle * i);
		double sine1 = sin(angle * (i + 1));
		double nx = (height / hyp)*cosine;
		double ny = radius / hyp;
		double nz = (hyp / radius)*sine;
		Point3D norms = Point3D(nx, ny, nz).unit();
		glVertex3f(center[0], yTop, center[0]);
		glNormal3f(norms[0], norms[1], norms[2]);
		glVertex3f(xRad * cosine1, yBot, zRad * sine1);
		glNormal3f(norms[0], norms[1], norms[2]);
		glVertex3f(xRad * cosine, yBot, zRad * sine);
		glNormal3f(norms[0], norms[1], norms[2]);

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