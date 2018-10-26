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

double RayTriangle::intersect( Ray3D ray , RayIntersectionInfo& iInfo , double mx )
{

	//Point3D h, s, q;
	//double a, f, u, v, w;
	//h = Point3D::CrossProduct(ray.direction, v2);
	//a = Point3D::Dot(v1, h);
	//if (abs(a) < RAYEPS) // The ray does not hit the triangle
	//	return -1;

	//f = 1.0 / a;
	//s = ray.position - this->v[0]->position;
	//u = f * Point3D::Dot(s, h);
	//if (u < 0 || u > 1.0)
	//	return -1;
	//q = Point3D::CrossProduct(s, v1);
	//v = f * Point3D::Dot(ray.direction, q);
	//if (v < 0 || u + v > 1.0)
	//	return -1;
	//double t = f * Point3D::Dot(v2, q);
	//if (t < RAYEPS) // ray intersection culling
	//	return -1;
	//if (mx >= 0 && t >= mx) // We do not have a valid intersection
	//	return -1;
	//w = 1 - u - v;
	//iInfo.normal = (this->v[0]->normal * u + this->v[1]->normal * v + this->v[2]->normal * w).unit();
	//return t;
}
BoundingBox3D RayTriangle::setBoundingBox( void )
{
	throw RayException( "RayTriangle::setBoundingBox undefined" );
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
