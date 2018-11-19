#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED
#include <Util/geometry.h>
#include "rayShape.h"

/** This class describes a sphere, and is represented by its center and radius. */
class RaySphere : public RayShape
{
	/** The OpenGL vertex array identifier */
	GLuint _vertexArrayID = 0;
	/** The OpenGL vertex buffer identifier */
	GLuint _vertexBufferID = 0;
	/** The OpenGL element buffer identifier */
	GLuint _elementBufferID = 0;
public:
	/** The center of the sphere */
	Point3D center;
	/** The radius of the sphere */
	double radius;
	/** The material associated with the sphere */
	class RayMaterial *material;
	
	/** The default constructore */
	RaySphere( void );

	/** This method reads in a RaySphere from a .ray file.
	  * A pointer to the material associated to the triangle is set.
	  */
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

#endif // SPHERE_INCLUDED

