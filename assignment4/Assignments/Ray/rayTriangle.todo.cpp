#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Util/exceptions.h>
#include "rayTriangle.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
void RayTriangle::initialize( void )
{
	static bool firstTime = true;
	if( firstTime ) Util::Warn( "RayTriangle::intialize undefined" );
	firstTime = false;
}
double RayTriangle::intersect( Ray3D ray , RayIntersectionInfo& iInfo , double mx )
{
	Util::Throw( "RayTriangle::intersect undefined" );
	return -1;
}
BoundingBox3D RayTriangle::setBoundingBox( void )
{
	Util::Throw( "RayTriangle::setBoundingBox undefined" );
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayTriangle::drawOpenGL( int materialIndex , GLSLProgram * glslProgram )
{
	Util::Throw( "RayTriangle::drawOpenGL undefined" );
	return -1;
}
