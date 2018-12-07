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

//Matrix3D::Matrix3D( const Quaternion& q )
//{
//	Util::Throw( "Matrix3D::Matrix3D undefined" );
//	(*this) = IdentityMatrix();
//}
//Matrix3D Matrix3D::closestRotation( void ) const
//{
//	Util::Throw( "Matrix3D::closestRotation undefined" );
//	return (*this);
//}
//Matrix3D Matrix3D::Exp( const Matrix3D& m , int iter )
//{
//	Util::Throw( "Matrix3D::Exp undefined" );
//	return IdentityMatrix();
//}
Matrix3D::Matrix3D(const Quaternion& q)
{
	Quaternion curr = q.unit();
	double r = curr.real;
	double i = curr.imag[0];
	double j = curr.imag[1];
	double k = curr.imag[2];
	m[0][0] = 1 - 2 * (j*j + k * k);
	m[1][0] = 2 * (i*j - r * k);
	m[2][0] = 2 * (i*k + r * j);
	m[0][1] = 2 * (i*j + r * k);
	m[1][1] = 1 - 2 * (i*i + k * k);
	m[2][1] = 2 * (j*k - r * i);
	m[0][2] = 2 * (i*k - r * j);
	m[1][2] = 2 * (j*k + r * i);
	m[2][2] = 1 - 2 * (i*i + j * j);
}

Matrix3D Matrix3D::closestRotation(void) const
{
	Matrix3D u, d, v;
	SVD(u, d, v);
	for (int r = 0; r < 3; r++) d(r, r) = (d(r, r) > 0) ? 1 : -1;
	d(2, 2) = (u * v).determinant();
	return u * d * v;
}
Matrix3D Matrix3D::Exp(const Matrix3D& m, int iter)
{
	Matrix3D e = IdentityMatrix();
	Matrix3D m_n = m;
	int k_factorial = 1;
	for (int i = 1; i <= iter;) {
		e += m_n / k_factorial;
		m_n *= m;
		k_factorial *= ++i;
	}
	return e.closestRotation();
}
