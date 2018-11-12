#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Util/geometry.h>
#include "rayPointLight.h"
#include "rayScene.h"

void RayPointLight::read( FILE* fp )
{
	if( fscanf( fp , " %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg" , &aColor[0] , &aColor[1] , &aColor[2] , &dColor[0] , &dColor[1] , &dColor[2] , &sColor[0] , &sColor[1] , &sColor[2] , &location[0] , &location[1] , &location[2] , &constAtten , &linearAtten , &quadAtten )!=15 )
	{
		throw RayException( std::string( "Failed to parse light_point\n" ) ); 
	}
}
void RayPointLight::write( FILE* fp )
{
	fprintf( fp , "#light_point\n" );
	fprintf( fp , "  %lg %lg %lg\n" , aColor[0] , aColor[1] , aColor[2] );
	fprintf( fp , "  %lg %lg %lg\n" , dColor[0] , dColor[1] , dColor[2] );
	fprintf( fp , "  %lg %lg %lg\n" , sColor[0] , sColor[1] , sColor[2] );
	fprintf( fp , "  %lg %lg %lg\n" , location[0] , location[1] , location[2] );
	fprintf( fp , "  %lg %lg %lg\n" , constAtten , linearAtten , quadAtten );
}
