#include <stdlib.h>
#include <math.h>

#include <SVD/SVDFit.h>
#include <SVD/MatrixMNTC.h>
#include <algorithm>

#include "geometry.h"

/////////////
// Point2D //
/////////////
Point2D::Point2D( void ) { p[0] = p[1] = 0; }
Point2D::Point2D( double x , double y ) { p[0]=x , p[1]=y; }
double&  Point2D::operator[] ( int i ){ return p[i]; }
double   Point2D::operator[] ( int i ) const { return p[i]; }
Point2D  Point2D::unit( void ) const { return (*this)/length(); }
Point2D  Point2D::operator -  ( void ) const { return (*this) * -1.; }
Point2D  Point2D::operator *  ( double s ) const { return Point2D( p[0]*s , p[1]*s ); }
Point2D  Point2D::operator /  ( double s ) const { return (*this) * (1./s); }
Point2D& Point2D::operator *= ( double s ){ p[0]*=s , p[1]*=s ; return *this; }
Point2D& Point2D::operator /= ( double s ){ return (*this)*=(1./s); }
double Point2D::Dot( const Point2D& p , const Point2D& q ){ return p[0]*q[0] + p[1]*q[1]; }
double Point2D::squareNorm( void ) const { return Dot( *this , *this ); }
double Point2D::length( void ) const { return sqrt( squareNorm() ); }


Point2D  Point2D::operator +  ( const Point2D& q ) const { return Point2D( p[0]+q.p[0] , p[1]+q.p[1] ); }
Point2D& Point2D::operator += ( const Point2D& q){ p[0]+=q.p[0] , p[1]+=q.p[1] ; return *this; }
Point2D  Point2D::operator -  ( const Point2D& q ) const { return Point2D( p[0]-q.p[0] , p[1]-q.p[1] ); }
Point2D& Point2D::operator -= ( const Point2D& q ){ p[0]-=q.p[0] , p[1]-=q.p[1] ; return *this; }
Point2D  Point2D::operator *  ( const Point2D& q ) const { return Point2D( p[0]*q.p[0] , p[1]*q.p[1] ); }
Point2D& Point2D::operator *= ( const Point2D& q ){	p[0]*=q.p[0] , p[1]*=q.p[1] ; return *this; }
Point2D  Point2D::operator /  ( const Point2D& q ) const { return Point2D( p[0]/q.p[0] , p[1]/q.p[1] ); }
Point2D& Point2D::operator /= ( const Point2D& q ){ p[0]/=q.p[0] , p[1]/=q.p[1] ; return *this; }

/////////////
// Point3D //
/////////////
Point3D::Point3D( void ){ p[0] = p[1] = p[2] = 0; }
Point3D::Point3D( double x , double y , double z ){ p[0]=x , p[1]=y , p[2]=z; }
double& Point3D::operator[] ( int i ) { return p[i]; }
double  Point3D::operator[] ( int i ) const { return p[i]; }
Point3D  Point3D::unit( void ) const { return (*this) / length(); }
Point3D  Point3D::operator -  ( void ) const { return (*this) * -1.; }
Point3D  Point3D::operator *  ( double s ) const { return Point3D( p[0]*s , p[1]*s , p[2]*s ); }
Point3D& Point3D::operator *= ( double s ){ p[0]*=s , p[1]*=s , p[2]*=s ; return *this; }
Point3D  Point3D::operator /  ( double s ) const { return (*this) * (1./s); }
Point3D& Point3D::operator /= ( double s ){ p[0]/=s , p[1]/=s , p[2]/=s ; return *this; }
double Point3D::Dot( const Point3D& p , const Point3D& q ){ return p[0]*q[0] + p[1]*q[1] + p[2]*q[2]; }
double Point3D::squareNorm( void ) const { return Dot( *this , *this ); }
double Point3D::length( void ) const {return sqrt( squareNorm() ); }

Point3D  Point3D::operator +  ( const Point3D& q ) const { return Point3D( p[0]+q.p[0] , p[1]+q.p[1] , p[2]+q.p[2] ); }
Point3D& Point3D::operator += ( const Point3D& q ){ p[0]+=q.p[0] , p[1]+=q.p[1] , p[2]+=q.p[2] ; return *this; }
Point3D  Point3D::operator -  ( const Point3D& q ) const { return Point3D( p[0]-q.p[0] , p[1]-q.p[1] , p[2]-q.p[2] ); }
Point3D& Point3D::operator -= ( const Point3D& q ){ p[0]-=q.p[0] , p[1]-=q.p[1] , p[2]-=q.p[2] ; return *this; }

Point3D Point3D::CrossProduct( const Point3D& p , const Point3D& q ){ return Point3D( p[1]*q[2] - p[2]*q[1] , -p[0]*q[2] + p[2]*q[0] , p[0]*q[1] - p[1]*q[0] ); }

Point3D  Point3D::operator *  ( const Point3D& q ) const { return Point3D( p[0]*q[0] , p[1]*q[1] , p[2]*q[2] ); }
Point3D& Point3D::operator *= ( const Point3D& q ){ p[0]*=q[0] , p[1]*=q[1] , p[2]*=q[2] ; return *this; }
Point3D  Point3D::operator /  ( const Point3D& q ) const { return Point3D( p[0]/q[0] , p[1]/q[1] , p[2]/q[2] ); }
Point3D& Point3D::operator /= ( const Point3D& q ){ p[0]/=q[0] , p[1]/=q[1] , p[2]/=q[2] ; return *this; }

/////////////
// Plane3D //
/////////////
Plane3D::Plane3D( void ) : normal(1,0,0) , distance(0) {}
Plane3D::Plane3D( const Point3D& n , const Point3D& p )
{
	normal = n.unit();
	distance = Point3D::Dot( normal , p );
}
Plane3D::Plane3D( const Point3D& p1 , const Point3D& p2 , const Point3D& p3 )
{
	normal = Point3D::CrossProduct( p2-p1 , p3-p1 ).unit();
	distance = -Point3D::Dot( normal , p1 );
}
void Plane3D::makePositive( const Point3D& p )
{
	if( (*this)(p)<0 ) normal =- normal , distance = -distance;
}
double Plane3D::operator()(const Point3D& p) const { return Point3D::Dot( normal , p ) + distance; }


///////////
// Ray3D //
///////////
Ray3D::Ray3D( void ) : position(0,0,0) , direction(1,0,0) {}
Ray3D::Ray3D( const Point3D& p , const Point3D& d ) : position(p) , direction(d) {}
Point3D Ray3D::operator() ( double s ) const { return position+direction*s; }
Ray3D  Ray3D::operator +  ( const Point3D& p ) const { return Ray3D( position+p , direction );}
Ray3D& Ray3D::operator += ( const Point3D& p ){ position += p ; return *this; }
Ray3D  Ray3D::operator -  ( const Point3D& p ) const { return Ray3D( position-p , direction );}
Ray3D& Ray3D::operator -= ( const Point3D& p ){ position -= p ; return *this; }

//////////////
// Matrix3D //
//////////////
Matrix3D::Matrix3D( void ){ memset( m , 0 , sizeof(double)*9 ); }

Matrix3D::Matrix3D( const Matrix4D& n ){ memcpy( m , n.m , sizeof(double)*9 ); }
double& Matrix3D::operator() ( int i , int j )       { return m[i][j]; }
double  Matrix3D::operator() ( int i , int j ) const { return m[i][j]; }

double Matrix3D::determinant( void ) const { return m[0][0] * ( m[1][1]*m[2][2] - m[1][2]*m[2][1] ) - m[1][0] * ( m[0][1]*m[2][2] - m[0][2]*m[2][1] ) + m[2][0] * ( m[0][1]*m[1][2] - m[0][2]*m[1][1] ); }
double Matrix3D::trace( void ) const { return m[0][0] + m[1][1] + m[2][2]; }
Matrix3D Matrix3D::operator - ( void ) const
{
	Matrix3D n;
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ;j++ ) n.m[i][j] = -m[i][j];
	return n;
}
Matrix3D  Matrix3D::operator * ( const Matrix3D& n ) const
{
	Matrix3D o;
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) for( int k=0 ; k<3 ; k++ ) o.m[i][j] += m[k][j] * n.m[i][k];
	return o;
}
Matrix3D& Matrix3D::operator *= ( const Matrix3D& m ){ *this = (*this) * m ; return *this; }
Matrix3D Matrix3D::transpose( void ) const
{
	Matrix3D n;
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) n.m[i][j] = m[j][i];
	return n;
}
Matrix3D  Matrix3D::operator + ( const Matrix3D& n ) const
{
	Matrix3D o;
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) o.m[i][j] = m[i][j] + n.m[i][j];
	return o;
}
Matrix3D& Matrix3D::operator += ( const Matrix3D& n )
{
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) m[i][j] += n.m[i][j];
	return *this;
}
Matrix3D  Matrix3D::operator - ( const Matrix3D& n ) const
{
	Matrix3D o;
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) o.m[i][j] = m[i][j] - n.m[i][j];
	return o;
}
Matrix3D& Matrix3D::operator -= ( const Matrix3D& n )
{
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) m[i][j] += n.m[i][j];
	return *this;
}

Matrix3D  Matrix3D::operator * ( double f ) const
{
	Matrix3D o;
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) o.m[i][j] = m[i][j] * f;
	return o;
}
Matrix3D& Matrix3D::operator *= ( double f )
{
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) m[i][j] *= f;
	return *this;
}
Matrix3D  Matrix3D::operator / ( double f ) const
{
	Matrix3D o;
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) o.m[i][j] = m[i][j] / f;
	return o;
}
Matrix3D& Matrix3D::operator /= ( double f )
{
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) m[i][j] *= f;
	return *this;
}

Matrix3D Matrix3D::inverse( void ) const
{
	Matrix3D n;
	n.m[0][0] =  ( m[1][1]*m[2][2] - m[1][2]*m[2][1] );
	n.m[0][1] = -( m[0][1]*m[2][2] - m[0][2]*m[2][1] );
	n.m[0][2] =  ( m[0][1]*m[1][2] - m[0][2]*m[1][1] );
	n.m[1][0] = -( m[1][0]*m[2][2] - m[1][2]*m[2][0] );
	n.m[1][1] =  ( m[0][0]*m[2][2] - m[0][2]*m[2][0] );
	n.m[1][2] = -( m[0][0]*m[1][2] - m[0][2]*m[1][0] );
	n.m[2][0] =  ( m[1][0]*m[2][1] - m[1][1]*m[2][0] );
	n.m[2][1] = -( m[0][0]*m[2][1] - m[0][1]*m[2][0] );
	n.m[2][2] =  ( m[0][0]*m[1][1] - m[0][1]*m[1][0] );
	double d = m[0][0]*n.m[0][0] + m[0][1]*n.m[1][0] + m[0][2]*n.m[2][0];
	return n/d;
}

Point3D Matrix3D::operator * ( const Point3D& p ) const
{
	Point3D q;
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) q[i] += m[j][i] * p[j];
	return q;
}
Ray3D Matrix3D::operator * (const Ray3D& r) const
{
	return Ray3D( (*this) * r.position , (*this)*r.direction );
}

Matrix3D Matrix3D::IdentityMatrix( void )
{
	Matrix3D m;
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) m(i,j) = (i==j) ? 1 : 0;
	return m;
}

Point3D Matrix3D::toEuler( void ) const
{
	Point3D e;
	e[1] = asin(-m[0][2]);
	if( sqrt(m[0][0]*m[0][0]+m[0][1]*m[0][1] )>0.000001 )
	{
		e[2] = atan2( m[0][1] , m[0][0] );
		e[0] = atan2( m[1][2] , m[2][2] );
	}
	else{
		e[2] = atan2( m[1][0] , m[0][2] );
		e[0] = 0;
	}
	return e;
}
Point3D Matrix3D::toEuler( const Point3D& e ) const
{
	Point3D euler=toEuler();
	for( int i=0 ; i<3 ; i++ )
	{
		double d = fabs( euler.p[i] - e.p[i] );
		double a = euler.p[i];
		if( euler.p[i]<e.p[i] )
		{
			while( euler.p[i]<e.p[i] )
			{
				euler.p[i] += 2*PI;
				if( fabs( euler.p[i]-e.p[i] )<d )
				{
					d=fabs( euler.p[i]-e.p[i] );
					a=euler.p[i];
				}
			}
		}
		else
		{
			while( euler.p[i]>e.p[i] )
			{
				euler.p[i] -= 2*PI;
				if( fabs( euler.p[i]-e.p[i] )<d )
				{
					d = fabs( euler.p[i]-e.p[i] );
					a = euler.p[i];
				}
			}
		}
		euler.p[i] = a;
	}
	return euler;
}
double Matrix3D::Dot( const Matrix3D& m , const Matrix3D& n )
{
	double dot = 0;
	for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) dot += m(i,j) * n(i,j);
	return dot;
}
double Matrix3D::squareNorm( void ) const { return Dot( *this , *this ); }
double Matrix3D::length( void ) const { return sqrt( squareNorm() ); }

void Matrix3D::SVD( Matrix3D& r1 , Matrix3D& d , Matrix3D& r2 ) const
{
	GXMatrixMNd M( 3 , 3 );
	GXMatrixMNd U, W, Vt;

	d = r1 = r2 = IdentityMatrix();

	for( int i=0 ; i<3 ; i++ ) for(  int j=0 ; j<3 ; j++ ) M(i,j) = m[j][i];
	SVDMat( M , U , W , Vt );  // M == U . DiagonalMatrix(W) . Vt
	for( int i=0 ; i<3 ; i++ )
	{
		for( int j=0 ; j<3 ; j++ )
		{
			r1(i,j) = U(j,i);
			r2(i,j) = Vt(j,i);
		}
		d(i,i) = W(i,0);
	}
	double t1,t2;
	t1=r1.determinant();
	t2=r2.determinant();
	if( t1<0 || t2<0 ) for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ )
	{
		if( t1<0 ) r1(i,j) *= -1;
		if( t2<0 ) r2(i,j) *= -1;
		if( t1*t2<0 ) d(i,j) *= -1;
	}
}

void Matrix3D::Factor( Matrix3D& rot , Matrix3D& sym ) const
{
	Matrix3D r1 , d , r2;
	SVD( r1 , d , r2 );
	rot = r1*r2;
	sym = r2.transpose() * d * r2;
}

// Code borrowed from:
// Linear Combination of Transformations
// Marc Alexa
Matrix3D Matrix3D::SquareRoot( const Matrix3D& m , double eps )
{
	Matrix3D X,Y;
	X = m;
	Y = IdentityMatrix();
	while( (X*X-m).squareNorm()>eps*eps )
	{
		Matrix3D iX = X.inverse();
		Matrix3D iY = Y.inverse();
		X = (X+iY) / 2;
		Y = (Y+iX) / 2;
	}
	return X;
}
Matrix3D Matrix3D::Log( const Matrix3D& m , double eps )
{
	Matrix3D I = IdentityMatrix();
	Matrix3D X , Z , A=m;
	int k=0;
	while( (A-I).squareNorm()>0.25 )
	{
		A = SquareRoot( A , eps );
		k++;
	}
	A = I-A;
	X = Z = A;
	int i = 1;
	while( Z.squareNorm()>eps*eps )
	{
		Z = Z*A;
		i++;
		X += Z*( 1.0/i );
	}
	return X * ( -pow( 2.0 , (double)k ) );
}
Matrix3D Matrix3D::symmetrize( void ) const { return ( (*this)+transpose() ) / 2; }
Matrix3D Matrix3D::skewSymmetrize( void ) const { return ( (*this)-transpose() ) / 2; }

//////////////
// Matrix4D //
//////////////
Matrix4D::Matrix4D( void ){ memset( m , 0 , sizeof(double)*16 ); }
Matrix4D::Matrix4D( const Matrix4D& n ) { memcpy( m , n.m , sizeof(double)*16 ); }
Matrix4D::Matrix4D( const Matrix3D& n ) : Matrix4D() { for( int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) m[i][j] = n.m[i][j]; }
Matrix4D::Matrix4D( const Matrix3D& n , const Point3D& p ) : Matrix4D( n ){ for( int i=0 ; i<3 ; i++ ) m[3][i] = p[i]; }

double& Matrix4D::operator() ( int i , int j ) { return m[i][j]; }
double  Matrix4D::operator() ( int i , int j ) const { return m[i][j]; }

double  Matrix4D::trace( void ) const { return m[0][0] + m[1][1] + m[2][2] + m[3][3]; }
double  Matrix4D::determinant( void ) const
{
	double d = 0;
	for( int i=0 ; i<4 ; i++ )
		if( (i%2)==0 ) d += subDet(i,0) * m[i][0];
		else           d -= subDet(i,0) * m[i][0];
	return d;
}
double Matrix4D::subDet( int c1 , int r1 , int c2 , int r2 ) const { return m[c1][r1]*m[c2][r2] - m[c1][r2]*m[c2][r1]; }
double Matrix4D::subDet( int c , int r ) const
{
	int c1 , r1 , c2 , r2 , row=0;
	double d=0 , sgn=1;
	if( row==r ) row++;
	for( int i=0 ; i<4 ; i++ )
	{
		if( i==c ) continue;
		c1 = 0;
		while( c1==i || c1==c ) c1++;
		c2 = c1+1;
		while( c2==i || c2==c ) c2++;
		r1 = 0;
		while( r1==row || r1==r ) r1++;
		r2 = r1+1;
		while( r2==row || r2==r ) r2++;
		
		d += sgn * m[i][row] * subDet( c1 , r1 , c2 , r2 );
		sgn *= -1;
	}
	return d;
}
Matrix4D Matrix4D::operator - ( void ) const
{
	Matrix4D n;
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) n.m[i][j] = -m[i][j];
	return n;
}

Matrix4D  Matrix4D::operator * ( const Matrix4D& n ) const
{
	Matrix4D o;
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) for( int k=0 ; k<4 ; k++ ) o.m[i][j] += m[k][j] * n.m[i][k];
	return o;
}
Matrix4D& Matrix4D::operator *= ( const Matrix4D& m )
{
	*this = (*this) * m;
	return *this;
}
Matrix4D Matrix4D::transpose( void ) const
{
	Matrix4D n;
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) n.m[i][j] = m[j][i];
	return n;
}
Matrix4D  Matrix4D::operator + ( const Matrix4D& n ) const
{
	Matrix4D o;
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) o.m[i][j] = m[i][j] + n.m[i][j];
	return o;
}
Matrix4D& Matrix4D::operator += ( const Matrix4D& n )
{
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) m[i][j] += n.m[i][j];
	return *this;
}
Matrix4D  Matrix4D::operator - ( const Matrix4D& n ) const
{
	Matrix4D o;
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) o.m[i][j] = m[i][j] - n.m[i][j];
	return o;
}
Matrix4D& Matrix4D::operator -= ( const Matrix4D& n )
{
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) m[i][j] -= n.m[i][j];
	return *this;
}

Matrix4D  Matrix4D::operator * ( double f ) const
{
	Matrix4D n;
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) n.m[i][j] = m[i][j] * f;
	return n;
}
Matrix4D& Matrix4D::operator *= ( double f )
{
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) m[i][j] *= f;
	return *this;
}
Matrix4D  Matrix4D::operator / ( double f ) const
{
	Matrix4D n;
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) n.m[i][j] = m[i][j] / f;
	return n;
}
Matrix4D& Matrix4D::operator /= ( double f )
{
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) m[i][j] /= f;
	return *this;
}

Matrix4D Matrix4D::inverse( void ) const
{
	Matrix4D m;
	double d = determinant();
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ )
		if( (i+j)%2==0 ) m(j,i) =  subDet(i,j) / d;
		else             m(i,j) = -subDet(j,i) / d;
	return m;
}

Point3D Matrix4D::operator * ( const Point3D& p ) const { return multPosition(p); }
Point3D Matrix4D::multPosition( const Point3D& p ) const
{
	Point3D q;
	double t=0;
	for( int i=0 ; i<3 ; i++ )
	{
		q[i] = m[3][i];
		for( int j=0 ; j<3 ; j++ ) q[i] += m[j][i]*p.p[j];
	}
	t = m[0][3]*p[0] + m[1][3]*p[1] + m[2][3]*p[2] + m[3][3];
	return q/t;
}
Point3D Matrix4D::multDirection( const Point3D& p ) const
{
	Point3D q;
	for(int i=0 ; i<3 ; i++ ) for( int j=0 ; j<3 ; j++ ) q[i] += m[j][i]*p.p[j];
	return q;
}
Point3D Matrix4D::multNormal( const Point3D& p ) const { return transpose().inverse().multDirection(p); }

Ray3D Matrix4D::operator * ( const Ray3D& r ) const
{
	Ray3D q;
	q.position = multPosition( r.position );
	q.direction = multDirection( r.direction );
	return q;
}

Matrix4D Matrix4D::IdentityMatrix( void )
{
	Matrix4D m;
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) m(i,j) = (i==j) ? 1 : 0;
	return m;
}
Point3D Matrix4D::toEuler( void ) const
{
	Point3D e;
	e[1] = asin(-m[0][2]);
	if( sqrt( m[0][0]*m[0][0] + m[0][1]*m[0][1] )>0.000001 )
	{
		e[2] = atan2( m[0][1] , m[0][0] );
	 	e[0] = atan2( m[1][2] , m[2][2] );
	}
	else
	{
		e[2] = atan2( m[1][0] , m[0][2] );
		e[0] = 0;
	}
	return e;
}
double Matrix4D::Dot( const Matrix4D& m , const Matrix4D& n )
{
	double dot = 0;
	for( int i=0 ; i<4 ; i++ ) for( int j=0 ; j<4 ; j++ ) dot += m(i,j)*n(i,j);
	return dot;
}
double Matrix4D::squareNorm( void ) const { return Dot( *this , *this ); }
double Matrix4D::length( void ) const { return sqrt( squareNorm() ); }

/* While these Exp and Log implementations are the direct implementations of the Taylor series, the Log
 * function tends to run into convergence issues so we use the other ones:*/

///////////////////
// BoundingBox3D //
///////////////////
BoundingBox3D::BoundingBox3D( void ){}
BoundingBox3D::BoundingBox3D( const Point3D& p1 , const Point3D& p2 )
{
	for( int i=0 ; i<3 ; i++ ) p[0][i] = std::min< double >( p1[i] , p2[i] ) , p[1][i] = std::max< double >( p1[i] , p2[i] );
}
BoundingBox3D::BoundingBox3D( const Point3D* pList , int pSize )
{
	if( pSize>0 )
	{
		p[0] = p[1] = pList[0];
		for( int i=1 ; i<pSize ; i++ ) for( int j=0 ; j<3 ; j++ ) p[0][j] = std::min< double >( p[0][j] , pList[i][j] ) , p[1][j] = std::max< double >( p[1][j] , pList[i][j] );
	}
}
BoundingBox3D BoundingBox3D::operator + ( const BoundingBox3D& b ) const
{
	Point3D pList[4];
	Point3D q;
	
	if( (b.p[0]-b.p[1]).squareNorm()==0.0 ) return *this;
	if( (p[0]-p[1]).squareNorm()==0.0 ) return b;
	pList[0]=p[0];
	pList[1]=p[1];
	pList[2]=b.p[0];
	pList[3]=b.p[1];
	return BoundingBox3D( pList , 4 );
}
BoundingBox3D& BoundingBox3D::operator += ( const BoundingBox3D& b )
{
	*this = *this + b;
	return *this;
}

BoundingBox3D operator * ( const Matrix4D& m , const BoundingBox3D& b )
{
	Point3D v[8];
	for( int i=0 ; i<2 ; i++ ) for( int j=0 ; j<2 ; j++ ) for( int k=0 ; k<2 ; k++ ) v[i+2*j+4*k] = m.multPosition( Point3D( b.p[i].p[0] , b.p[j].p[1] , b.p[k].p[2] ) );
	return BoundingBox3D( v , 8 );
}

////////////////
// Quaternion //
////////////////
Quaternion::Quaternion( void ) : real(0) {}
Quaternion::Quaternion( const double r , const Point3D& i ) : real(r) , imag(i) {}

Quaternion::Quaternion( const Matrix3D& rot )
{
	real = 0.5 * sqrt( 1 + rot.trace() );
	imag[0] = -1./(4*real) * ( rot(2,1)-rot(1,2) );
	imag[1] = -1./(4*real) * ( rot(0,2)-rot(2,0) );
	imag[2] = -1./(4*real) * ( rot(1,0)-rot(0,1) );
}
Quaternion::Quaternion( const Matrix3D& rot , const Quaternion& quat ) : Quaternion(rot)
{
	if( ( (*this)-quat).squareNorm()>( (*this)+quat ).squareNorm() ) (*this) *= -1;
}
double Quaternion::Dot( const Quaternion& p , const Quaternion& q ){ return p.real*q.real + Point3D::Dot( p.imag , q.imag ); }
double Quaternion::squareNorm( void ) const { return Dot( *this , *this ); }
double Quaternion::length( void ) const { return sqrt( squareNorm() ); }
Quaternion Quaternion::unit( void ) const { return (*this)/length(); }
Quaternion Quaternion::operator- ( void ) const { return Quaternion( -real , -imag ); }
Quaternion Quaternion::conjugate( void ) const { return Quaternion( real , -imag ); }
Quaternion Quaternion::inverse( void ) const {return conjugate()/squareNorm(); }
Quaternion  Quaternion::operator *  ( double scale ) const { return Quaternion( real*scale , imag*scale ); }
Quaternion& Quaternion::operator *= ( double scale ){ real*=scale , imag*= scale ; return *this; }
Quaternion  Quaternion::operator /  ( double scale ) const { return Quaternion( real/scale , imag/scale ); }
Quaternion& Quaternion::operator /= ( double scale ){ real/=scale , imag/=scale ; return *this; }
Quaternion  Quaternion::operator +  ( const Quaternion& q ) const { return Quaternion( real+q.real , imag+q.imag ); }
Quaternion& Quaternion::operator += ( const Quaternion& q ){ real+=q.real , imag+=q.imag ; return *this; }
Quaternion  Quaternion::operator -  ( const Quaternion& q ) const {return Quaternion( real-q.real , imag-q.imag ); }
Quaternion& Quaternion::operator -= ( const Quaternion& q ){ real-=q.real , imag-=q.imag ; return *this; }
Quaternion  Quaternion::operator *  ( const Quaternion& q ) const { return Quaternion( real*q.real - Point3D::Dot( imag , q.imag ) , imag*q.real + q.imag*real + Point3D::CrossProduct( imag , q.imag ) ); }
Quaternion& Quaternion::operator *= ( const Quaternion& q ){ *this = (*this) * q ; return *this; }
Quaternion  Quaternion::operator /  ( const Quaternion& q ) const { return (*this) * q.inverse();}
Quaternion& Quaternion::operator /= ( const Quaternion& q ){ return (*this) *= q.inverse(); }


//////////////////////////////
// EulerAngleAndTranslation //
//////////////////////////////
EulerAnglesAndTranslation::EulerAnglesAndTranslation( void ){}
EulerAnglesAndTranslation::EulerAnglesAndTranslation( const Point3D& e , const Point3D& t ) : eulerAngles(e) , translation(t) {}
EulerAnglesAndTranslation::EulerAnglesAndTranslation( const Matrix4D& m )
{
	for( int i=0 ; i<3 ; i++ ) translation[i] = m(3,i);
	eulerAngles = Matrix3D(m).closestRotation().toEuler();
}
EulerAnglesAndTranslation::EulerAnglesAndTranslation( const Matrix4D& m , const EulerAnglesAndTranslation& e )
{
	for( int i=0 ; i<3 ; i++ ) translation[i] = m(3,i);
	eulerAngles = Matrix3D(m).closestRotation().toEuler(e.eulerAngles);
}
EulerAnglesAndTranslation EulerAnglesAndTranslation::operator -( void ) const { return EulerAnglesAndTranslation( -eulerAngles , -translation ); }
EulerAnglesAndTranslation  EulerAnglesAndTranslation::operator *  ( double scale ) const { return EulerAnglesAndTranslation( eulerAngles*scale , translation*scale ); }
EulerAnglesAndTranslation& EulerAnglesAndTranslation::operator *= ( double scale ) { eulerAngles *= scale , translation *= scale ; return *this; }
EulerAnglesAndTranslation  EulerAnglesAndTranslation::operator /  ( double scale ) const { return EulerAnglesAndTranslation( eulerAngles/scale , translation/scale ); }
EulerAnglesAndTranslation& EulerAnglesAndTranslation::operator /= ( double scale ) { eulerAngles /= scale , translation /= scale ; return *this; }
EulerAnglesAndTranslation  EulerAnglesAndTranslation::operator +  ( const EulerAnglesAndTranslation& et ) const { return EulerAnglesAndTranslation( eulerAngles + et.eulerAngles , translation + et.translation ); }
EulerAnglesAndTranslation& EulerAnglesAndTranslation::operator += ( const EulerAnglesAndTranslation& et ) { eulerAngles += et.eulerAngles , translation += et.translation ; return *this; }
EulerAnglesAndTranslation  EulerAnglesAndTranslation::operator -  ( const EulerAnglesAndTranslation& et ) const { return EulerAnglesAndTranslation( eulerAngles - et.eulerAngles , translation - et.translation ); }
EulerAnglesAndTranslation& EulerAnglesAndTranslation::operator -= ( const EulerAnglesAndTranslation& et ) { eulerAngles -= et.eulerAngles , translation -= et.translation ; return *this; }

////////////////////////////
// RotationAndTranslation //
////////////////////////////
RotationAndTranslation::RotationAndTranslation( void ) : rotation( Matrix3D::IdentityMatrix() ){}
RotationAndTranslation::RotationAndTranslation( const Matrix3D& r , const Point3D& t ) : rotation(r) , translation(t) {}
RotationAndTranslation::RotationAndTranslation( const Matrix4D& m )
{
	for( int i=0 ; i<3 ; i++ ) translation[i] = m(3,i);
	rotation=Matrix3D(m).closestRotation();
}
RotationAndTranslation  RotationAndTranslation::operator - ( void ) const { return RotationAndTranslation( -rotation , -translation ); }
RotationAndTranslation  RotationAndTranslation::operator *  ( double scale ) const { return RotationAndTranslation( rotation*scale , translation*scale ); }
RotationAndTranslation& RotationAndTranslation::operator *= ( double scale ) { rotation *= scale , translation *= scale ; return *this; }
RotationAndTranslation  RotationAndTranslation::operator /  ( double scale ) const { return RotationAndTranslation( rotation/scale , translation/scale ); }
RotationAndTranslation& RotationAndTranslation::operator /= ( double scale ) { rotation /= scale , translation /= scale ; return *this; }
RotationAndTranslation  RotationAndTranslation::operator +  ( const RotationAndTranslation& rt ) const { return RotationAndTranslation( rotation + rt.rotation , translation + rt.translation ); }
RotationAndTranslation& RotationAndTranslation::operator += ( const RotationAndTranslation& rt ){ rotation += rt.rotation , translation += rt.translation ; return *this; }
RotationAndTranslation  RotationAndTranslation::operator -  ( const RotationAndTranslation& rt ) const { return RotationAndTranslation( rotation - rt.rotation , translation - rt.translation ); }
RotationAndTranslation& RotationAndTranslation::operator -= ( const RotationAndTranslation& rt ){ rotation -= rt.rotation , translation -= rt.translation ; return *this; }

////////////////////////////////
// 	LogRotationAndTranslation //
////////////////////////////////
LogRotationAndTranslation::LogRotationAndTranslation( void ){}
LogRotationAndTranslation::LogRotationAndTranslation( const Matrix3D& s , const Point3D& t ) : skewSymmetric(s) , translation(t) {}
LogRotationAndTranslation::LogRotationAndTranslation( const Matrix4D& m )
{
	for( int i=0 ; i<3 ; i++ ) translation[i] = m(3,i);
	skewSymmetric = Matrix3D::Log( Matrix3D(m).closestRotation() ).skewSymmetrize();
}
LogRotationAndTranslation::LogRotationAndTranslation( const Matrix4D& m , const LogRotationAndTranslation& lg )
{
	for( int i=0 ; i<3 ; i++ ) translation[i] = m(3,i);
	Matrix3D s1 = lg.skewSymmetric.skewSymmetrize();
	Matrix3D s2 = Matrix3D::Log( Matrix3D(m).closestRotation() ).skewSymmetrize();

	double n1 = sqrt( s1.squareNorm()/2 ) / 2;
	double n2 = sqrt( s2.squareNorm()/2 ) / 2;
	// If the matrix is the identity matrix
	if( n2==0 )
	{
		// If the matrix we want to get close to is also the identity matrix
		if( n1==0 ) skewSymmetric = s1;
		else
		{
			//s1*2kPI/n1 will exponentate to the identity
			//so, we need to choose k so that 2kPI is as close to n1 as possible 
			s1 /= n1;
			n1 /= 2*PI;
			n1 = ( (int)(n1+.5) ) * 2 * PI;
			skewSymmetric = s1*n1;
		}
	}
	else
	{
		s2 /= n2;
		// s2*(n2+2kPI) will exponentiate to what we need
		// need to solve for k minimizing ||s1-s2*(n2+2kPI)||
		Matrix3D t;
		skewSymmetric = s2*n2;
		double dd , d = ( s1-s2*n2 ).squareNorm();
		t = s2 * ( n2 + 2*PI );
		while( ( dd=(s1-t).squareNorm())<d )
		{
			d = dd;
			skewSymmetric = t;
			t += s2 * (2*PI);
		}
		t = s2 * (n2-2*PI);
		while( ( dd=(s1-t).squareNorm() )<d )
		{
			d = dd;
			skewSymmetric = t;
			t -= s2 * (2*PI);
		}
	}
}

LogRotationAndTranslation  LogRotationAndTranslation::operator - ( void ) const { return LogRotationAndTranslation( -skewSymmetric , -translation ); }
LogRotationAndTranslation  LogRotationAndTranslation::operator *  ( double scale ) const { return LogRotationAndTranslation( skewSymmetric*scale , translation*scale ); }
LogRotationAndTranslation& LogRotationAndTranslation::operator *= ( double scale ) { skewSymmetric *= scale , translation *= scale ; return *this; }
LogRotationAndTranslation  LogRotationAndTranslation::operator /  ( double scale ) const { return LogRotationAndTranslation( skewSymmetric/scale , translation/scale ); }
LogRotationAndTranslation& LogRotationAndTranslation::operator /= ( double scale ) { skewSymmetric /= scale , translation /= scale ; return *this; }
LogRotationAndTranslation  LogRotationAndTranslation::operator +  ( const LogRotationAndTranslation& lt ) const { return LogRotationAndTranslation( skewSymmetric + lt.skewSymmetric , translation + lt.translation ); }
LogRotationAndTranslation& LogRotationAndTranslation::operator += ( const LogRotationAndTranslation& lt ) { skewSymmetric += lt.skewSymmetric , translation += lt.translation ; return *this; }
LogRotationAndTranslation  LogRotationAndTranslation::operator -  ( const LogRotationAndTranslation& lt ) const { return LogRotationAndTranslation( skewSymmetric - lt.skewSymmetric , translation + lt.translation ); }
LogRotationAndTranslation& LogRotationAndTranslation::operator -= ( const LogRotationAndTranslation& lt ) { skewSymmetric -= lt.skewSymmetric , translation -= lt.translation ; return *this; }

///////////////////////////////
// 	QuaternionAndTranslation //
///////////////////////////////
QuaternionAndTranslation::QuaternionAndTranslation( void ){}
QuaternionAndTranslation::QuaternionAndTranslation( const Quaternion& q , const Point3D& t ) : quaternion(q) , translation(t) {}

QuaternionAndTranslation::QuaternionAndTranslation( const Matrix4D& m )
{
	for( int i=0 ; i<3 ; i++ ) translation[i] = m(3,i);
	quaternion = Quaternion( Matrix3D(m).closestRotation() ).unit();
}
QuaternionAndTranslation::QuaternionAndTranslation( const Matrix4D& m , const QuaternionAndTranslation& q )
{
	for( int i=0 ; i<3 ; i++ ) translation[i] = m(3,i);
	quaternion = Quaternion( Matrix3D(m).closestRotation() , q.quaternion ).unit();
}
QuaternionAndTranslation QuaternionAndTranslation::operator - ( void ) const { return QuaternionAndTranslation( -quaternion , -translation ); }
QuaternionAndTranslation  QuaternionAndTranslation::operator *  ( double scale ) const { return QuaternionAndTranslation( quaternion*scale , translation*scale ); }
QuaternionAndTranslation& QuaternionAndTranslation::operator *= ( double scale ) { quaternion *= scale , translation *= scale ; return *this; }
QuaternionAndTranslation  QuaternionAndTranslation::operator /  ( double scale ) const { return QuaternionAndTranslation( quaternion/scale , translation/scale ); }
QuaternionAndTranslation& QuaternionAndTranslation::operator /= ( double scale ) { quaternion /= scale , translation /= scale ; return *this; }
QuaternionAndTranslation  QuaternionAndTranslation::operator +  ( const QuaternionAndTranslation& qt ) const { return QuaternionAndTranslation( quaternion + qt.quaternion , translation + qt.translation ); }
QuaternionAndTranslation& QuaternionAndTranslation::operator += ( const QuaternionAndTranslation& qt ) { quaternion += qt.quaternion , translation += qt.translation ; return *this; }
QuaternionAndTranslation  QuaternionAndTranslation::operator -  ( const QuaternionAndTranslation& qt ) const { return QuaternionAndTranslation( quaternion - qt.quaternion , translation - qt.translation ); }
QuaternionAndTranslation& QuaternionAndTranslation::operator -= ( const QuaternionAndTranslation& qt ) { quaternion -= qt.quaternion , translation -= qt.translation ; return *this; }
