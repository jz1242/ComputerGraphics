#include "rayScene.h"
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <math.h>


///////////////////////
// Ray-tracing stuff //
///////////////////////

Point3D RayScene::Reflect( Point3D v , Point3D n )
{
	throw RayException( "RayScene::Reflect undefined" );
	return Point3D();
}

bool RayScene::Refract( Point3D v , Point3D n , double ir , Point3D& refract )
{
	throw RayException( "RayScene::Refract undefined" );
	return 0;
}

Ray3D RayScene::GetRay( RayCamera* camera , int i , int j , int width , int height )
{
	throw RayException( "RayScene::GetRay undefined" );
	return Ray3D();
}

Point3D RayScene::getColor( Ray3D ray , int rDepth , Point3D cLimit )
{
	fprintf( stderr , "[WARNING] RayScene::getColor undefined\n" );
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RayMaterial::drawOpenGL( GLSLProgram * glslProgram )
{
	throw RayException( "RayMaterial::drawOpenGL undefined" );
}
void RayTexture::setUpOpenGL( void )
{
	throw RayException( "RayTexture::setUpOpenGL undefined" );
}
