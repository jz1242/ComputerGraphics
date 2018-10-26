#include <string>
#include "rayShape.h"
#include "rayCamera.h"

void RayCamera::read( FILE* fp )
{
	if( fscanf( fp , " %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg" , &position[0] , &position[1] , &position[2] , &direction[0] , &direction[1] , &direction[2] , &up[0] , &up[1] , &up[2] , &heightAngle )!=10 )
	{
		throw RayException( std::string( "Failed to parse camera for Box\n" ) );
	}
	direction = direction.unit();
	up = up.unit();
	right = Point3D::CrossProduct( direction , up );
}
void RayCamera::write( FILE* fp )
{
	fprintf( fp , "#camera\n");
	fprintf( fp , "   %lg %lg %lg\n" , position[0] , position[1] , position[2] );
	fprintf( fp , "   %lg %lg %lg\n" , direction[0] , direction[1] , direction[2] );
	fprintf( fp , "   %lg %lg %lg\n" , up[0] , up[1] , up[2] );
	fprintf( fp , "   %lg\n" , heightAngle );
}
