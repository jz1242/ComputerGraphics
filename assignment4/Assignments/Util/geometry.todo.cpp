#include <stdlib.h>
#include <math.h>

#include <SVD/SVDFit.h>
#include <SVD/MatrixMNTC.h>
#include <Util/exceptions.h>

#include "geometry.h"

///////////////////////
// Ray-tracing stuff //
///////////////////////
double BoundingBox3D::intersect( const Ray3D& ray ) const
{
	Util::Throw( "BoundingBox3D::intersect undefined" );
	return -1;
}

/////////////////////
// Animation stuff //
/////////////////////
Matrix3D::Matrix3D( const Point3D& e )
{
	double cx = cos(e[0]);
	double sx = sin(e[0]);
	double cy = cos(e[1]);
	double sy = sin(e[1]);
	double cz = cos(e[2]);
	double sz = sin(e[2]);


	m[0][0] = cy * cz;
	m[0][1] = cy * sz;
	m[0][2] = -sy;


	m[1][0] = sx * sy*cz - cx * sz;
	m[1][1] = sx * sy*sz + cx * cz;
	m[1][2] = sx * cy;


	m[2][0] = cx * sy*cz + sx * sz;
	m[2][1] = cx * sy*sz - sx * cz;
	m[2][2] = cx * cy;
}

Matrix3D::Matrix3D( const Quaternion& q )
{
	//column major
	Quaternion unitQ = q.unit();
	double a = unitQ.real;
	double b = unitQ.imag[0];
	double c = unitQ.imag[1];
	double d = unitQ.imag[2];
	m[0][0] = 1 - 2 * (pow(c, 2)) - 2 * (pow(d, 2));
	m[0][1] = 2 * (b*c) + 2 * (a*d);
	m[0][2] = 2 * (b*d) - 2 * (a*c);

	m[1][0] = 2 * (b*c) - 2 * (a*d);
	m[1][1] = 1 - 2 * (pow(b, 2)) - 2 * (pow(d, 2));
	m[1][2] = 2 * (c*d) + 2 * (a*b);

	m[2][0] = 2 * (b*d) + 2 * (a*c);
	m[2][1] = 2 * (c* d) - 2 * (a*b);
	m[2][2] = 1 - 2 * (pow(b, 2)) - 2 * (pow(c, 2));
}
Matrix3D Matrix3D::closestRotation( void ) const
{
	Matrix3D u, sigma, v;
	SVD(u, sigma, v);
	for (int r = 0; r < 2; r++) {
		sigma(r, r) = 1;
	}
	Matrix3D mult = u * v;
	sigma(2, 2) = mult.determinant();
	return u * sigma * v;
}
Matrix3D Matrix3D::Exp( const Matrix3D& m , int iter )
{
	Matrix3D res = IdentityMatrix();
	Matrix3D powM = m;
	double val = 1;
	for (int i = 1; i <= iter; i++) {
		val /= i;
		res += (powM * val);
		powM *= m;
	}
	return res;
}
