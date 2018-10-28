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
	Point3D n = plane.normal;
	Point3D pos = ray.position;
	Point3D dir = ray.direction;

	double t = -Point3D::Dot(n, pos) - plane.distance;
	double den = Point3D::Dot(dir, n);
	if (den > -FLT_EPSILON && den < FLT_EPSILON) {
		return -1;
	}
	double t_dist = t / Point3D::Dot(dir, n);
	if (t_dist < 0 || (mx != -1 && t_dist > mx)) {
		return -1;
	}
	Point3D curr_p = pos + dir * (t_dist);

	Point3D v0top = curr_p - v[0]->position;
	double v1dotv2 = Point3D::Dot(v1, v2);
	double curr_p_v1 = Point3D::Dot(v0top, v1);
	double curr_p_v2 = Point3D::Dot(v0top, v2);
	double v1norm2 = v1.squareNorm();
	double v2norm2 = v2.squareNorm();

	double inv_den = 1 / (v1norm2 * v2norm2 - v1dotv2 * v1dotv2);
	double u = (v2norm2 * curr_p_v1 - v1dotv2 * curr_p_v2) * inv_den;
	double v = (v1norm2 * curr_p_v2 - v1dotv2 * curr_p_v1) * inv_den;
	double w = 1 - u - v;

	// Update if required
	if (w < 0 || u < 0 || v < 0) {
		return -1;
	}
	iInfo.iCoordinate = curr_p;
	iInfo.normal = (this->v[0]->normal * w + this->v[1]->normal * u + this->v[2]->normal * v).unit();
	return t_dist;
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
