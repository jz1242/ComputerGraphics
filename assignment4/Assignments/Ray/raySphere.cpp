#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Util/exceptions.h>
#include "raySphere.h"
#include "rayScene.h"

RaySphere::RaySphere( void ) : radius(0) , material(NULL){}
void RaySphere::read( FILE* fp , int* index )
{
	if( fscanf( fp ," %d %lg %lg %lg %lg" , index , &center[0] , &center[1] , &center[2] , &radius )!=5 ) Util::Throw( "Failed to parse shape_sphere" ); 
}

void  RaySphere::write( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#shape_sphere %d\n" , material->index );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %lg %lg %lg\n" , center[0] , center[1] , center[2] );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %lg\n" , radius );
}
