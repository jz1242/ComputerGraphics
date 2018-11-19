#ifndef TRIANGLE_INCLUDED
#define TRIANGLE_INCLUDED
#include <Util/geometry.h>
#include "rayShape.h"
#include "rayScene.h"

/** This class stores information about a vertex . */
class RayVertex
{
public:
	/** The index of the vertex within the scene */
	int index;
	/** The position of the vertex */
	Point3D position;
	/** The normal at the vertex */
	Point3D normal;
	/** The texture coordinates at the vertex */
	Point2D texCoordinate;

	/** This method reads in the vertex information from a .ray file.*/
	void read( FILE* fp );
	/** This method writes out the vertex information to a .ray file. If no file is specified,
	* the vertex is written out to stdout. */
	void write( FILE* fp=stdout );
};

/** This class represents a triangle and is specified by three pointers to
  * the three vertices that define it.
  */
class RayTriangle : public RayShape
{
	Point3D v1;
	Point3D v2;
	Plane3D plane;
	void initialize( void );
public:
	/** The vertices of the triangle */
	class RayVertex* v[3];

	RayTriangle( void );

	/** This method reads in a RayTriangle from a .ray file.
	* The vertex positions are determined by reading in the vertex indices from the file
	* and then looking them up in the vertex list vList
	*/
	void read( FILE* fp , std::vector< RayVertex >& vList );

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

	void addTrianglesOpenGL( std::vector< RayShape::TriangleIndex >& triangles );

	int drawOpenGL( int materialIndex , GLSLProgram * glslProgram );
};
#endif // TRIANGLE_INCLUDED


