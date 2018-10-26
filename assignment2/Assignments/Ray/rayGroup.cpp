#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "rayTriangle.h"
#include "rayGroup.h"

/////////////////
// RayShapeHit //
/////////////////
int RayShapeHit::Compare( const void* v1 , const void* v2 )
{
	RayShapeHit *i1 = (RayShapeHit*)v1;
	RayShapeHit *i2 = (RayShapeHit*)v2;
	if     ( i1->t<i2->t ) return -1;
	else if( i2->t<i1->t ) return  1;
	else                   return  0;
}
//////////////
// RayGroup //
//////////////
void RayGroup::addShape( RayShape* s )
{
	shapes.push_back( s );
	hits.resize( shapes.size() );
}
void RayGroup::write( int indent , FILE* fp )
{
	writeBegin( indent , fp );
	for( int i=0 ; i<shapes.size() ; i++ ) shapes[i]->write( indent+2 , fp );
	writeEnd( indent , fp );
}

void RayGroup::setUpOpenGL( int cplx , bool setBufferObjects )
{
	openGLComplexity = cplx;
	for( int i=0 ; i<shapes.size() ; i++ ) shapes[i]->setUpOpenGL( cplx , setBufferObjects );
}

void RayGroup::addTrianglesOpenGL( std::vector< RayShape::TriangleIndex >& triangles )
{
	for( int i=0 ; i<shapes.size() ; i++ ) shapes[i]->addTrianglesOpenGL( triangles );
}

/////////////////////
// TrivialRayGroup //
/////////////////////
void TrivialRayGroup::writeBegin( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#trivial_begin\n" );
}
void TrivialRayGroup::writeEnd( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#trivial_end\n" );
}
Matrix4D TrivialRayGroup::getMatrix( void ){ return Matrix4D::IdentityMatrix(); }
Matrix4D TrivialRayGroup::getInverseMatrix( void ){ return Matrix4D::IdentityMatrix(); }
Matrix4D TrivialRayGroup::getNormalMatrix( void ){ return Matrix4D::IdentityMatrix(); }

//////////////////////////
// TriangleListRayGroup //
//////////////////////////
void TriangleListRayGroup::writeBegin( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#triangle_list_begin %d\n" , material->index );
	for( int i=0 ; i<vertices.size() ; i++ ) vertices[i].write( fp );
}
void TriangleListRayGroup::writeEnd( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#triangle_list_end\n" );
}
void TriangleListRayGroup::read( FILE* fp , int* index )
{
	if( fscanf( fp ," %d" , index )!=1 )
	{
		throw RayException( std::string( "Failed to parse triangle_list_begin for TriangleListRayGroup\n" ) );
	}
}

////////////////////
// StaticRayGroup //
////////////////////
StaticRayGroup::StaticRayGroup( void ) : RayGroup() { localTransform=Matrix4D::IdentityMatrix(); }

void StaticRayGroup::read( FILE* fp )
{
	if( fscanf( fp , " %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg" , 
		&localTransform(0,0) , &localTransform(1,0) ,
		&localTransform(2,0) , &localTransform(3,0) ,
		&localTransform(0,1) , &localTransform(1,1) ,
		&localTransform(2,1) , &localTransform(3,1) ,
		&localTransform(0,2) , &localTransform(1,2) ,
		&localTransform(2,2) , &localTransform(3,2) ,
		&localTransform(0,3) , &localTransform(1,3) ,
		&localTransform(2,3) , &localTransform(3,3) )!=16 )
	{
		throw RayException( std::string( "Failed to parse group_begin\n" ) );
	}
	set();
}
void StaticRayGroup::set( Matrix4D m )
{
	localTransform = m;
	set();
}

void StaticRayGroup::writeBegin( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#group_begin\n" );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %lg %lg %lg %lg\n" , localTransform(0,0) , localTransform(1,0) , localTransform(2,0) , localTransform(3,0) );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %lg %lg %lg %lg\n" , localTransform(0,1) , localTransform(1,1) , localTransform(2,1) , localTransform(3,1) );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %lg %lg %lg %lg\n" , localTransform(0,2) , localTransform(1,2) , localTransform(2,2) , localTransform(3,2) );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %lg %lg %lg %lg\n" , localTransform(0,3) , localTransform(1,3) , localTransform(2,3) , localTransform(3,3) );
}
void StaticRayGroup::writeEnd( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#group_end\n" );
}

Matrix4D StaticRayGroup::getMatrix( void ){ return localTransform; }
Matrix4D StaticRayGroup::getInverseMatrix( void ){ return inverseTransform; }
Matrix4D StaticRayGroup::getNormalMatrix( void ){ return normalTransform; }

//////////////////////////
// ParametrizedRayGroup //
//////////////////////////
void ParametrizedRayGroup::writeBegin( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#joint_begin %s\n" , pName );
}
void ParametrizedRayGroup::writeEnd( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#joint_end\n" );
}
Matrix4D ParametrizedRayGroup::getInverseMatrix( void ){ return getMatrix().inverse(); }
Matrix4D ParametrizedRayGroup::getNormalMatrix( void ){ return getMatrix().inverse().transpose(); }


//////////////////////////
// ParametrizedMatrix4D //
//////////////////////////
Matrix4D ParametrizedMatrix4D::getMatrix( void ){ return *value; }
