#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayScene.h"
#include "raySphere.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RaySphere::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	throw RayException("RaySphere::intersect undefined");
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
	throw RayException("RaySphere::setUpOpenGL undefined");
}

int RaySphere::drawOpenGL(int materialIndex, GLSLProgram * glslProgram)
{
	throw RayException("RaySphere::drawOpenGL undefined");
	return -1;
}