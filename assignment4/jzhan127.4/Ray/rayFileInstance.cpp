#include <stdio.h>
#include <stdlib.h>
#include "rayFileInstance.h"

RayFileInstance::RayFileInstance( RayFile* rf ){ rayFile=rf; }
void RayFileInstance::write( int indent , FILE* fp )
{
	for( int i=0 ; i<indent ; i++ ) fprintf( fp , " " );
	fprintf( fp , "#ray_file_instance %d\n" , rayFile->index );
}
double RayFileInstance::intersect( Ray3D ray , RayIntersectionInfo& iInfo , double mx )
{
	return rayFile->scene->group->intersect(ray,iInfo,mx);
}

BoundingBox3D RayFileInstance::setBoundingBox( void )
{
	bBox=rayFile->scene->group->setBoundingBox();
	return bBox;
}

void RayFileInstance::setUpOpenGL( int cplx , bool setCallList )
{
	openGLComplexity = cplx;
}

int RayFileInstance::drawOpenGL( int materialIndex , GLSLProgram * glslProgram ){ rayFile->scene->group->drawOpenGL( -1 , glslProgram ) ; return -1; }
