#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Util/geometry.h>
#include "rayDirectionalLight.h"
#include "rayScene.h"

void RayDirectionalLight::read( FILE* fp )
{
	if( fscanf( fp , " %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg" , &aColor[0] , &aColor[1] , &aColor[2] , &dColor[0] , &dColor[1] , &dColor[2] , &sColor[0] , &sColor[1] , &sColor[2] , &direction[0] , &direction[1] , &direction[2] )!=12 )
	{
		throw RayException( std::string( "Failed to parse light_dir\n" ) );
	}
	direction = direction.unit();
}
void RayDirectionalLight::write( FILE* fp )
{
	fprintf( fp , "#light_dir\n" );
	fprintf( fp , "  %lg %lg %lg\n" , aColor[0] , aColor[1] , aColor[2] );
	fprintf( fp , "  %lg %lg %lg\n" , dColor[0] , dColor[1] , dColor[2] );
	fprintf( fp , "  %lg %lg %lg\n" , sColor[0] , sColor[1] , sColor[2] );
	fprintf( fp , "  %lg %lg %lg\n" , direction[0] , direction[1] , direction[2] );
}
