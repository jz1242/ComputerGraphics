#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <Util/exceptions.h>
#include "rayScene.h"
#include "raySphere.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RaySphere::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	Util::Throw("Undefined");
	return -1;
}

BoundingBox3D RaySphere::setBoundingBox(void)
{
	Point3D p(radius, radius, radius);
	bBox = BoundingBox3D(center + p, center - p);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
void RaySphere::setUpOpenGL(int cplx, bool setBufferObjects)
{
	openGLComplexity = cplx;
	_vertexArrayID = _elementBufferID = _vertexBufferID = 0;
	//Util::Throw("Undefined");
}

int RaySphere::drawOpenGL(int materialIndex, GLSLProgram * glslProgram)
{
	if (materialIndex != material->index) material->drawOpenGL(glslProgram);
	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);
	GLUquadric* q = gluNewQuadric();
	gluSphere(q, radius, 2 * openGLComplexity, openGLComplexity);
	gluDeleteQuadric(q);
	glPopMatrix();
	return material->index;
}