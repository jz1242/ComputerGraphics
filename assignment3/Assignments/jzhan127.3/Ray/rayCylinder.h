#ifndef CYLINDER_INCLUDED
#define CYLINDER_INCLUDED
#include <Util/geometry.h>
#include "rayShape.h"

/** This class represents a cylinder whose central axis is parallel to the y-axis, 
  * and is defined by the center of the cylinder, the height from the top cap
  * to the bottom cap, and the radius of the cylinder. */
class RayCylinder : public RayShape
{
	/** The OpenGL vertex array identifier */
	GLuint _vertexArrayID = 0;
	/** The OpenGL vertex buffer identifier */
	GLuint _vertexBufferID = 0;
	/** The OpenGL element buffer identifier */
	GLuint _elementBufferID = 0;
public:
	/** The center of the cylinder */
	Point3D center;
	/** The hieght of the cylinder */
	double height;
	/** The radius of the cylinder */
	double radius;
	/** The material associated with the cylinder */
	class RayMaterial *material;
	
	RayCylinder( void );

	/** This method reads in a RayCylinder from a .ray file. A pointer to the material associated to the triangle is set.*/
	void read( FILE* fp , int* materialIndex );

	void write( int indent , FILE* fp=stdout );
	
	///////////////////////
	// Ray-Tracing stuff //
	///////////////////////
	double intersect( Ray3D ray , struct RayIntersectionInfo& iInfo , double mx=-1 );
	
	BoundingBox3D setBoundingBox( void );

	//////////////////
	// OpenGL stuff //
	//////////////////
	void setUpOpenGL( int cplx , bool setBufferObjects );

	int drawOpenGL( int materialIndex , GLSLProgram * glslProgram );
};

#endif // CYLINDER_INCLUDED 
