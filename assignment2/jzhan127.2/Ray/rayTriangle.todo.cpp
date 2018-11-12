#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rayTriangle.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
void RayTriangle::initialize(void)
{
	plane = Plane3D(v[0]->position, v[1]->position, v[2]->position);
	v1 = v[1]->position - v[0]->position;
	v2 = v[2]->position - v[0]->position;
}

double RayTriangle::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	Point3D dir = ray.direction;
	Point3D h = Point3D::CrossProduct(dir, v2);
	double a = Point3D::Dot(v1, h);
	if (a > -RAYEPS && a < RAYEPS){
		return false;
	}
	double f = 1.0 / a;
	Point3D s = ray.position - this->v[0]->position;
	double u = f * Point3D::Dot(s, h);
	if (u > 1 || u < 0) {
		return -1;
	}
	Point3D q = Point3D::CrossProduct(s, v1);
	double v = f * Point3D::Dot(dir, q);
	if (u + v > 1 || v < 0) {
		return -1;
	}

	double t = f * Point3D::Dot(v2, q);
	double w = 1 - u - v;
	Point3D n = this->v[0]->normal * u + this->v[1]->normal * v + this->v[2]->normal * w;
	iInfo.normal = n.unit();
	if (mx >= 0 && t >= mx)
		return -1;
	return t;
}
BoundingBox3D RayTriangle::setBoundingBox( void )
{
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayTriangle::drawOpenGL( int materialIndex , GLSLProgram * glslProgram )
{
	throw RayException( "RayTriangle::drawOpenGL undefined" );
	return -1;
}
