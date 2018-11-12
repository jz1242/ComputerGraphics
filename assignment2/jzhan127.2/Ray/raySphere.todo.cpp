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
	double ret = -1;
	if (sol_1 > 0) {
		ret = sol_1;
	}
	if (sol_2 > 0) {
		ret = sol_2;
	}
	if ((mx > 0 && ret >= mx) || ret < 0) {
		return -1.0;
	}
	iInfo.iCoordinate = ray.position + ray.direction * ret;
	iInfo.material = this->material;
	iInfo.normal = (iInfo.iCoordinate - center).unit();
	return ret;
}

BoundingBox3D RaySphere::setBoundingBox( void )
{
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
