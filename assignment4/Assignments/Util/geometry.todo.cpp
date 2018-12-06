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
	Util::Throw( "Matrix3D::Matrix3D undefined" );
	(*this) = IdentityMatrix();
}

Matrix3D::Matrix3D( const Quaternion& q )
{
	Util::Throw( "Matrix3D::Matrix3D undefined" );
	(*this) = IdentityMatrix();
}
Matrix3D Matrix3D::closestRotation( void ) const
{
	Util::Throw( "Matrix3D::closestRotation undefined" );
	return (*this);
}
Matrix3D Matrix3D::Exp( const Matrix3D& m , int iter )
{
	Util::Throw( "Matrix3D::Exp undefined" );
	return IdentityMatrix();
}
