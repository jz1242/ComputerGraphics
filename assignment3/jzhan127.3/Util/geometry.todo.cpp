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
	throw GeometryException( "BoundingBox3D::intersect undefined" );
	return -1;
}

/////////////////////
// Animation stuff //
/////////////////////
Matrix3D::Matrix3D( const Point3D& e )
{
	throw GeometryException( "Matrix3D::Matrix3D undefined" );
	(*this) = IdentityMatrix();
}

Matrix3D::Matrix3D( const Quaternion& q )
{
	throw GeometryException( "Matrix3D::Matrix3D undefined" );
	(*this) = IdentityMatrix();
}
Matrix3D Matrix3D::closestRotation( void ) const
{
	throw GeometryException( "Matrix3D::closestRotation undefined" );
	return (*this);
}
Matrix3D Matrix3D::Exp( const Matrix3D& m , int iter )
{
	throw GeometryException( "Matrix3D::Exp undefined" );
	return IdentityMatrix();
}
