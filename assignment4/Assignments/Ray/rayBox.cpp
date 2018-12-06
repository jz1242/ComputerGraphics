#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Util/exceptions.h>
#include "rayBox.h"
#include "rayScene.h"

RayBox::RayBox( void ) : material(NULL) {}

void RayBox::read( FILE* fp , int* index )
{
	if( fscanf( fp , " %d %lg %lg %lg %lg %lg %lg" , index , &center[0] , &center[1] , &center[2] , &length[0] , &length[1] , &length[2] )!=7 )
	{
		Util::Throw( "Failed to parse shape_box\n" );
	}
}
void RayBox::write( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#shape_box %d\n" , material->index );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %lg %lg %lg\n" , center[0] , center[1] , center[2] );
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "  %lg %lg %lg\n" , length[0] , length[1] , length[2] );
}

//////////////////
// OpenGL stuff //
//////////////////
void RayBox::setUpOpenGL( int cplx , bool setBufferObjects )
{
	openGLComplexity = cplx;
}
