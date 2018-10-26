#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rayCone.h"
#include "rayScene.h"


RayCone::RayCone( void ) : height(0) , radius(0) , material(NULL) {}
void RayCone::read( FILE* fp , int* index )
{
	if( fscanf( fp , " %d %lg %lg %lg %lg %lg" , index , &center[0] , &center[1] , &center[2] , &radius , &height )!=6 )
	{
		throw RayException( std::string( "Failed to parse shape_cone\n" ) );
	}
}

void  RayCone::write( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#shape_cone %d\n" , material->index );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf(fp,"  %lg %lg %lg\n" , center[0] , center[1] , center[2] );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf(fp,"  %lg %lg\n" , radius , height );
}
