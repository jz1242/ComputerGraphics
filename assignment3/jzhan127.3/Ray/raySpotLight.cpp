#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Util/geometry.h>
#include "raySpotLight.h"
#include "rayScene.h"

void RaySpotLight::read( FILE* fp )
{
	double *l = &location[0] , *d = &direction[0];
	if( fscanf( fp , " %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg" , &aColor[0] , &aColor[1] , &aColor[2] , &dColor[0] , &dColor[1] , &dColor[2] , &sColor[0] , &sColor[1] , &sColor[2] , l+0 , l+1 , l+2 , d+0 , d+1 , d+2 , &constAtten , &linearAtten , &quadAtten , &cutOffAngle , &dropOffRate )!=20 )
	{
		throw RayException( std::string( "Failed to parse light_spot\n") );
	}
	direction = direction.unit();
}
void RaySpotLight::write( FILE* fp )
{
	fprintf( fp , "#light_spot\n" );
	fprintf( fp , "  %lg %lg %lg\n" , aColor[0] , aColor[1] , aColor[2] );
	fprintf( fp , "  %lg %lg %lg\n" , dColor[0] , dColor[1] , dColor[2] );
	fprintf( fp , "  %lg %lg %lg\n" , sColor[0] , sColor[1] , sColor[2] );
	fprintf( fp , "  %lg %lg %lg\n" , location[0] , location[1] , location[2] );
	fprintf( fp , "  %lg %lg %lg\n" , direction[0] , direction[1] , direction[2] );
	fprintf( fp , "  %lg %lg %lg\n" , constAtten , linearAtten , quadAtten );
	fprintf( fp , "  %lg %lg\n" , cutOffAngle , dropOffRate );
}
