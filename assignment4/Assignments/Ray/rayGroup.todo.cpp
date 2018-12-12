#include <stdlib.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <Util/exceptions.h>
#include "rayGroup.h"
#include "rayTriangle.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayGroup::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	Util::Throw("Undefined");
	return -1;
}

BoundingBox3D RayGroup::setBoundingBox(void)
{
	Point3D *pList = new Point3D[shapes.size() * 2];
	BoundingBox3D tBBox;
	for (int i = 0; i < shapes.size(); i++)
	{
		tBBox = shapes[i]->setBoundingBox();
		pList[2 * i] = tBBox.p[0];
		pList[2 * i + 1] = tBBox.p[1];
	}
	tBBox = BoundingBox3D(pList, (int)shapes.size() * 2);

	delete[] pList;
	bBox = getMatrix() * tBBox;
	return bBox;
}

bool StaticRayGroup::set(void)
{
	static bool firstTime = true;
	if (firstTime) //Util::Warn("Undefined");
	firstTime = false;
	return true;
}

//////////////////
// OpenGL stuff //
//////////////////

int RayGroup::drawOpenGL(int materialIndex, GLSLProgram * glslProgram)
{

	Matrix4D matrix = getMatrix();
	GLdouble m[16];
	for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) m[i + j * 4] = matrix(j, i);
	glPushMatrix();
	glMultMatrixd(m);
	for (int i = 0; i < shapes.size(); i++) materialIndex = shapes[i]->drawOpenGL(materialIndex, glslProgram);
	glPopMatrix();
	return materialIndex;
}

//////////////////////////
// TriangleListRayGroup //
//////////////////////////
////////////////////////
//  Ray-tracing stuff //
////////////////////////
double TriangleListRayGroup::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	Util::Throw("Undefined");
	return -1;
}

int TriangleListRayGroup::drawOpenGL(int materialIndex, GLSLProgram * glslProgram)
{
	Util::Throw("Undefined");
	return -1;
}

void TriangleListRayGroup::setUpOpenGL(int cplx, bool setBufferObjects)
{
	_vertexArrayID = _vertexBufferID = _elementBufferID = 0;
	//Util::Throw("Undefined");
}

/////////////////////
// Animation Stuff //
/////////////////////
Matrix4D ParametrizedEulerAnglesAndTranslation::getMatrix(void)
{
	Matrix3D euler = Matrix3D(this->value->eulerAngles);
	Point3D translate = this->value->translation;
	return Matrix4D(euler, translate);
}
Matrix4D ParametrizedClosestRotationAndTranslation::getMatrix(void)
{
	Matrix3D closestRot = this->value->rotation.closestRotation();
	Point3D translate = this->value->translation;
	return Matrix4D(closestRot, translate);
}
Matrix4D ParametrizedRotationLogarithmAndTranslation::getMatrix(void)
{
	int iter = 30;
	Matrix3D exp = Matrix3D::Exp(value->skewSymmetric, iter);
	Point3D translate = this->value->translation;
	return Matrix4D(exp, translate);
}
Matrix4D ParametrizedQuaternionAndTranslation::getMatrix(void)
{
	Matrix3D quaternion = Matrix3D(this->value->quaternion);
	Point3D translate = this->value->translation;
	return Matrix4D(quaternion, translate);
}
