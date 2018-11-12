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
double clamp(double val) {
	if (val > 1) {
		return 1;
	}
	if (val < 0) {
		return 0;
	}
	return val;
}

Point3D RayScene::Reflect( Point3D v , Point3D n )
{
	v = -v.unit();
	n = n.unit();
	double alpha = Point3D::Dot(v, n);
	if (alpha > 1) {
		alpha = 1;
	}
	if (alpha < 0) {
		alpha = 0;
	}
	return (n * alpha * 2 - v).unit();
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
	if (group->intersect(ray, iInfo, -1) > 0) {
		Point3D ret_val = iInfo.material->emissive;
		for (int i = 0; i < lights.size(); i++) {
			int inp = 0;
			if (lights[i]->isInShadow(iInfo, group, inp) == false) {
				ret_val += lights[i]->getAmbient(camera->position, iInfo) 
					+ lights[i]->getDiffuse(camera->position, iInfo)
					+ lights[i]->getSpecular(camera->position, iInfo);
			}
		}
		if (rDepth > 0) {
			Point3D specularity = iInfo.material->specular;
			Point3D n = iInfo.normal;
			Point3D v = ray.direction;
			Point3D ref = Reflect(v, n);
			Ray3D ray = Ray3D(iInfo.iCoordinate + ref * RAYEPS, ref.unit());
			ret_val += specularity*getColor(ray, rDepth--, cLimit);
		}
		ret_val[0] = clamp(ret_val[0]);
		ret_val[1] = clamp(ret_val[1]);
		ret_val[2] = clamp(ret_val[2]);
		return ret_val;

	}
	return Point3D(0, 0, 0);

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
