#include <stdlib.h>
#include <math.h>

#include <SVD/SVDFit.h>
#include <SVD/MatrixMNTC.h>

#include "geometry.h"

///////////////////////
// Ray-tracing stuff //
///////////////////////
double BoundingBox3D::intersect( const Ray3D& ray ) const
{
	return -1;
}

/////////////////////
// Animation stuff //
/////////////////////
Matrix3D::Matrix3D( const Point3D& e )
{
#ifdef USE_SOLUTION
	Matrix3D x , y , z;

	x(1,1) =  cos( e[0] );
	x(2,1) = -sin( e[0] );
	x(1,2) =  sin( e[0] );
	x(2,2) =  cos( e[0] );

	y(0,0) =  cos( e[1] );
	y(2,0) =  sin( e[1] );
	y(0,2) = -sin( e[1] );
	y(2,2) =  cos( e[1] );

	z(0,0) =  cos( e[2] );
	z(1,0) = -sin( e[2] );
	z(0,1) =  sin( e[2] );
	z(1,1) =  cos( e[2] );

	(*this) = z * y * x;
#else // !USE_SOLUTION
	(*this) = IdentityMatrix();
#endif // USE_SOLUTION
}

Matrix3D::Matrix3D( const Quaternion& q )
{
#ifdef USE_SOLUTION
	(*this) = IdentityMatrix();
	m[0][0] = 1. - 2.*q.imag[1]*q.imag[1] - 2.*q.imag[2]*q.imag[2];
	m[1][0] =      2.*q.imag[0]*q.imag[1] - 2.*q.real   *q.imag[2];
	m[2][0] =      2.*q.imag[0]*q.imag[2] + 2.*q.real   *q.imag[1];
	m[0][1] =      2.*q.imag[0]*q.imag[1] + 2.*q.real   *q.imag[2];
	m[1][1] = 1. - 2.*q.imag[0]*q.imag[0] - 2.*q.imag[2]*q.imag[2];
	m[2][1] =      2.*q.imag[1]*q.imag[2] - 2.*q.real   *q.imag[0];
	m[0][2] =      2.*q.imag[0]*q.imag[2] - 2.*q.real   *q.imag[1];
	m[1][2] =      2.*q.imag[1]*q.imag[2] + 2.*q.real   *q.imag[0];
	m[2][2] = 1. - 2.*q.imag[0]*q.imag[0] - 2.*q.imag[1]*q.imag[1];
#else // !USE_SOLUTION
	(*this) = IdentityMatrix();
#endif // USE_SOLUTION
}
Matrix3D Matrix3D::closestRotation( void ) const
{
#ifdef USE_SOLUTION
	Matrix3D r1 , d , r2;
	SVD( r1 , d , r2 );

	for( int i=0 ; i<3 ; i++ )
	{
		if( d(i,i)<0. ) d(i,i) = -1.;
		else            d(i,i) =  1.;
	}
	if( d.determinant()<0 ) d(2,2) = -d(2,2);

	return r1 * d * r2;
#else // !USE_SOLUTION
	return (*this);
#endif // USE_SOLUTION
}
Matrix3D Matrix3D::Exp( const Matrix3D& m , int iter )
{
#ifdef USE_SOLUTION
	double factor = 1.;
	Matrix3D M , _M = IdentityMatrix();

	for( int i=0 ; i<iter ; i++ )
	{
		M += _M/factor;
		_M *= m;
		factor *= (i+1);
	}
	return M;
#else // !USE_SOLUTION
	return IdentityMatrix();
#endif // USE_SOLUTION
}
