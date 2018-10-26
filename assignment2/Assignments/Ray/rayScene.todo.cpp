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
	Ray3D* ray = new Ray3D();
	Point3D start = camera->position;
	ray->position = start;
	double angle = tan(camera->heightAngle / 2);
	double x = (2 * ((i + 0.5) / width) - 1) * camera->aspectRatio * angle;
	double y = (2 * ((j + 0.5) / height) - 1) * angle;
	Point3D v = camera->right*x + camera->up*y + camera->direction;
	ray->direction = v.unit();
	return *ray;
}

Point3D RayScene::getColor( Ray3D ray , int rDepth , Point3D cLimit )
{
	RayIntersectionInfo iInfo;
	double ret = group->intersect(ray, iInfo, -1);
	if (ret > 0) {
		return Point3D(1, 1, 1);
	}
	return Point3D(0,0,0);
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
