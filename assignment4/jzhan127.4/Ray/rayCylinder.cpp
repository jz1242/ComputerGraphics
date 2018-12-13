#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Util/exceptions.h>
#include "rayCylinder.h"
#include "rayScene.h"


RayCylinder::RayCylinder( void ) : height(0) , radius(0) , material(NULL) {}

void RayCylinder::read( FILE* fp , int* index )
{
	if( fscanf( fp ," %d %lg %lg %lg %lg %lg" , index , &center[0] , &center[1] , &center[2] , &radius , &height )!=6 ) Util::Throw( "Failed to parse shape_cylinder\n" );
}

void RayCylinder::write( int indent , FILE *fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#shape_cylinder %d\n" , material->index );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %lg %lg %lg\n" , center[0] , center[1] , center[2] );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %lg %lg\n" , radius , height );
}
