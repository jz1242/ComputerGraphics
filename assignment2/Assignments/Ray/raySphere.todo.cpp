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
double RaySphere::intersect( Ray3D ray , RayIntersectionInfo& iInfo , double mx )
{
	double a = 1;
	double r2 = radius * radius;
	double b = 2 * Point3D::Dot(ray.direction, ray.position - center);
	double c = (ray.position - center).squareNorm() - r2;
	double sol_1 = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
	double sol_2 = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);
	if (sol_1 > 0) {
		return sol_1;
	}
	if (sol_2 > 0) {
		return sol_2;
	}
	return -1;
}

BoundingBox3D RaySphere::setBoundingBox( void )
{
	throw RayException( "RaySphere::setBoundingBox undefined" );
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
void RaySphere::setUpOpenGL( int cplx , bool setBufferObjects )
{
	openGLComplexity = cplx;
	_vertexArrayID = _elementBufferID = _vertexBufferID = 0;
	throw RayException( "RaySphere::setUpOpenGL undefined" );
}

int RaySphere::drawOpenGL( int materialIndex , GLSLProgram * glslProgram )
{
	throw RayException( "RaySphere::drawOpenGL undefined" );
	return -1;
}
