#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Util/exceptions.h>
#include "rayTriangle.h"


///////////////
// RayVertex //
///////////////
void RayVertex::read( FILE* fp )
{
	double size;
	double *p = &position[0] , *n = &normal[0] , *tc = &texCoordinate[0];
	if( fscanf( fp , " %lg %lg %lg %lg %lg %lg %lg %lg" , p+0 , p+1 , p+2 , n+0 , n+1 , n+2 , tc+0 , tc+1 )!=8 ) Util::Throw( "Failed to parse vertex" );
	size = normal.length();
	if( size==0.) fprintf( stderr , "No normal specified for vertex\n");
	else normal = normal/size;
}
void RayVertex::write( FILE* fp )
{
	fprintf( fp , "#vertex\n");
	fprintf( fp , "   %lg %lg %lg\n" , position[0] , position[1] , position[2] );
	fprintf( fp , "   %lg %lg %lg\n" , normal[0] , normal[1] , normal[2] );
	fprintf( fp , "   %lg %lg\n" , texCoordinate[0] , texCoordinate[1] );
}

/////////////////
// RayTriangle //
/////////////////
RayTriangle::RayTriangle( void ) { v[0] = v[1] = v[2] = NULL; }
void RayTriangle::read( FILE* fp , std::vector< RayVertex >& vList )
{
	int v1 , v2 , v3;
	if( fscanf( fp ," %d %d %d" , &v1 , &v2 , &v3 )!=3 ) Util::Throw( "Failed to parse shape_triangle for Triangle" );
	if( v1<0 || v1>=vList.size() || v2<0 || v2>=vList.size() || v3<0 || v3>=vList.size() ) Util::Throw( "Vertex index not within bounds for Triangle" );
	if( v1==v2 || v1==v3 || v2==v3 )
	{
		Util::Throw( "Vertices must be distinct in Triangle ( %d %d %d )" , v1 , v2 , v3 );
	}
	v[0] = &vList[v1];
	v[1] = &vList[v2];
	v[2] = &vList[v3];

	initialize();
}

void RayTriangle::write( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#shape_triangle\n" );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %d %d %d\n" , v[0]->index , v[1]->index , v[2]->index );
}

void RayTriangle::setUpOpenGL( int cplx , bool setBufferObjects )
{
	openGLComplexity = cplx;
}

void RayTriangle::addTrianglesOpenGL( std::vector< RayShape::TriangleIndex >& triangles ){ triangles.push_back( RayShape::TriangleIndex( v[0]->index , v[1]->index , v[2]->index ) ); }
