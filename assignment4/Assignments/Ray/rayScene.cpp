#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Util/exceptions.h>
#include <Image/bmp.h>
#include "rayScene.h"
#include "rayPointLight.h"
#include "rayDirectionalLight.h"
#include "raySpotLight.h"
#include "rayFileInstance.h"
#include "raySphere.h"
#include "rayBox.h"
#include "rayCone.h"
#include "rayCylinder.h"
#include "rayTriangle.h"
#include "rayGroup.h"

const static int BUF_SIZE=500;

/////////////////
// RayMaterial //
/////////////////
RayMaterial::RayMaterial( void ) : index(-1) , specularFallOff(0) , refind(1) , tex(NULL) { foo[0]=0; }
void RayMaterial::read( FILE* fp , int& tIdx )
{
	double *e = &emissive[0] , *a = &ambient[0] , *d = &diffuse[0] , *s = &specular[0] , *f = &specularFallOff , *t = &transparent[0] , *r = &refind;
	if( fscanf( fp , " %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %d !%s!" , e+0 , e+1 , e+2 , a+0 , a+1 , a+2 , d+0 , d+1 , d+2 , s+0 , s+1 , s+2 , f , t+0 , t+1 , t+2 , r , &tIdx , foo )!=19 )
		Util::Throw( "Failed to parse material" );
	else if( foo[strlen(foo)-1]=='!' ) foo[strlen(foo)-1] = '\0';
}
void RayMaterial::write( FILE* fp )
{
	int tIdx;
	
	if( !tex ) tIdx = -1;
	else       tIdx = tex->index;
	fprintf( fp , "#material\n");
	fprintf( fp , "   %lg %lg %lg\n" , emissive[0] , emissive[1] , emissive[2] );
	fprintf( fp , "   %lg %lg %lg\n" , ambient[0] , ambient[1] , ambient[2] );
	fprintf( fp , "   %lg %lg %lg\n" , diffuse[0] , diffuse[1] , diffuse[2] );
	fprintf( fp , "   %lg %lg %lg %lg\n" , specular[0] , specular[2] , specular[2] , specularFallOff );
	fprintf( fp , "   %lg %lg %lg\n" , transparent[0] , transparent[1] , transparent[2] );
	fprintf( fp , "   %lg\n" , refind );
	fprintf( fp , "   %d\n" , tIdx );
	fprintf( fp , "   !%s!\n" , foo );
}

////////////////
// RayTexture //
////////////////
RayTexture::RayTexture( void ) : index(0) , img(NULL) { filename[0] = 0; }
RayTexture::~RayTexture( void ){ if( img ) delete img; }

void RayTexture::read( FILE* f )
{
	if( fscanf( f , " %s" , filename )!= 1 ) Util::Throw( "Failed to parse texture" ); 
	img = new Image32();
	if( !img ) Util::Throw( "Failed to allocate image" );
	img->read( filename );
}
void RayTexture::write( FILE* fp )
{
	fprintf( fp , "#texture\n" );
	fprintf( fp , "   %s\n" , filename );
}

///////////////
// RayShader //
///////////////
RayShader::RayShader( void ) : glslProgram(NULL) { vertexShaderFilename[0] = fragmentShaderFilename[0] = 0; }
RayShader::~RayShader( void ){ if( glslProgram ) delete glslProgram; }

void RayShader::read( FILE* f )
{
	if( fscanf( f , " %s %s" , vertexShaderFilename , fragmentShaderFilename )!= 2 ) Util::Throw( "Failed to parse shader" );
	glslProgram = new GLSLProgram( vertexShaderFilename , fragmentShaderFilename );
	if( !glslProgram ) Util::Throw( "Failed to construct shader" );
}
void RayShader::write( FILE* fp )
{
	fprintf( fp , "#shader\n" );
	fprintf( fp , "   %s %s\n" , vertexShaderFilename , fragmentShaderFilename );
}

/////////////
// RayFile //
/////////////
RayFile::~RayFile( void ){ if( scene ) delete scene; }
void RayFile::read( FILE* fp , int transformType )
{
	scene = new RayScene();
	if( !scene ) Util::Throw( "Failed to allocate scene" );
	if( fscanf( fp , " %s" , filename )!=1 )
	{
		delete scene;
		scene = NULL;
		Util::Throw( "Failed to parse ray_file" );
	}
	scene->read( filename , transformType );
}
void RayFile::write( FILE* fp )
{
	fprintf( fp , "#ray_file\n" );
	fprintf( fp , "   %s\n" , filename );
}

//////////////
// RayScene //
//////////////
RayScene::RayScene( void ) : background(1.,1.,1.) , group(NULL) , camera(NULL) , shader(NULL) {}
RayMaterial* RayScene::getMaterial( int index )
{
	if( index<0 || index>=_materials.size() ) return NULL;
	return _materials[index];
}
RayScene::~RayScene( void )
{
	for( int i=0 ; i<lights.size() ; i++ ) delete lights[i];

	for( int i=0 ; i<_materials.size() ; i++ ) delete _materials[i];
	for( int i=0 ; i<_textures.size() ; i++ ) delete _textures[i];
	for( int i=0 ; i<_rayFiles.size() ; i++ ) delete _rayFiles[i];
	for( int i=0 ; i<_keyFileData.size() ; i++ ) delete _keyFileData[i];
	if( group ) delete group;
	if( camera ) delete camera;
}
void RayScene::write( char* fileName )
{
	if( !fileName ) write( stdout );
	else
	{
		FILE* fp = fopen( fileName , "w" );
		if( !fp ) Util::Throw( "Failed to open file for writing: %s" , fileName );
		write( fp );
		fclose( fp );
	}
}
void RayScene::write( FILE* fp )
{
	if( camera ) camera->write(fp);
	if( background[0] || background[1] || background[2] ) fprintf( fp , "#background\n   %lg %lg %lg\n" , background[0] , background[1] , background[2] );
	for( int i=0 ; i<lights.size() ; i++ ) lights[i]->write(fp);
	for( int i=0 ; i<_textures.size() ; i++ ) _textures[i]->write(fp);
	for( int i=0 ; i<_materials.size() ; i++ ) _materials[i]->write(fp);
	for( int i=0 ; i<_rayFiles.size() ; i++ ) _rayFiles[i]->write(fp);
	for( int i=0 ; i<group->shapes.size() ; i++ ) group->shapes[i]->write( 0 , fp );
}

void RayScene::read( char* fileName , int transformType )
{
	FILE* fp = fopen( fileName , "r" );
	if( !fp ) Util::Throw( "Failed to open file for reading: %s" , fileName );
	try
	{
		read( fp , transformType );
	}
	catch( Util::Exception &e )
	{
		fprintf( stderr , "%s\n" , e.what() );
		Util::Throw( "in file: %s" , fileName );
	}
	fclose( fp );
}
void RayScene::read( FILE* fp , int transformType )
{
	char keyword[BUF_SIZE];
	int cmndCtr = 0;
	bool inShapes = false , backFlag = false , ambFlag = false , shaderFlag = false;
	RayGroup* root;
	
	group = new StaticRayGroup();
	group->set( Matrix4D::IdentityMatrix() );
	root = group;
		
	while( fscanf( fp , " #%s" , keyword )==1 )
	{
		cmndCtr++;
		
		// We try to read out the camera information. We make sure that we have
		// not begun describing the scene graph yet, and we ignore the camera if 
		// it is the second one specified.
		if( !strcmp( keyword , "camera" ) )
		{
			if( inShapes ) Util::Throw( "[# %d] not a valid directive within a group" , cmndCtr );
			if( camera ) Util::Warn( "[# %d] ignoring extra camera" , cmndCtr );
			else
			{
				camera = new RayCamera();
				if( !camera ) Util::Throw( "[# %d] failed to allocate camera" , cmndCtr );
				camera->read( fp );
			}
		}
		
		// Next we try to read out the environment information. Again, we make
		// sure that we have not begun describing the scene graph yet and we 
		// ignore all but the first background color and ambient color 
		// specifications.
		else if( !strcmp( keyword , "background" ) )
		{
			if( inShapes ) Util::Throw( "[# %d] not a valid directive within a group" , cmndCtr );
			if( backFlag==1 ) Util::Warn( "[# %d] ignoring extra background" , cmndCtr );
			else
			{
				backFlag = true;
				if( fscanf( fp , " %lg %lg %lg" , &(background[0]) , &(background[1]) , &(background[2]) )!=3 ) Util::Throw( "[# %d] failed to parse background settings" , cmndCtr );
			}
		}
		
		//Next we try read out the next textures. We make sure that we have
		//not begun describing the scene graph. We make sure that we are not
		//adding more textures then we promised and we make sure that all the
		//file I/O works as it should.
		else if( !strcmp( keyword , "texture" ) )
		{
			if( inShapes ) Util::Throw( "[# %d] not a valid directive within a group" , cmndCtr );
			RayTexture* rayTexture = new RayTexture();
			if( !rayTexture ) Util::Throw( "[# %d] could not allocate RayTexture" , cmndCtr );
			rayTexture->read( fp );
			rayTexture->index = (int)_textures.size();
			_textures.push_back( rayTexture );
		}

		//Next we try read out the next material. We make sure that we have
		//not begun describing the scene graph. We make sure that we are not
		//adding more materials then we promised.
		else if( !strcmp( keyword , "material" ) )
		{
			if( inShapes ) Util::Throw( "[# %d] not a valid directive within a group" , cmndCtr );
			RayMaterial* rayMaterial = new RayMaterial();
			if( !rayMaterial ) Util::Throw( "[# %d] could not allocate RayMaterial" , cmndCtr );
			int tIdx;
			rayMaterial->read( fp , tIdx );
			rayMaterial->tex = (RayTexture*)(size_t)tIdx;
			rayMaterial->index = (int)_materials.size();
			_materials.push_back( rayMaterial );
		}
		
		//Next we try to read out the next rayFile
		else if( !strcmp( keyword , "ray_file" ) )
		{
			if( inShapes ) Util::Throw( "[# %d] not a valid directive within a group" , cmndCtr );
			RayFile* rayFile = new RayFile();
			if( !rayFile ) Util::Throw( "[# %d] could not allocate RayFile" , cmndCtr );
			rayFile->read( fp , transformType );
			rayFile->index = (int)_rayFiles.size();
			_rayFiles.push_back( rayFile );
		}
		
		//Next we try read out the next light. We make sure that we have
		//not begun describing the scene graph. We make sure that we are not
		//adding more materials then we promised.
		else if( !strcmp( keyword,"light_point" ) || !strcmp( keyword , "light_dir" ) || !strcmp( keyword , "light_spot" ) )
		{
			if( inShapes ) Util::Throw( "[# %d] not a valid directive within a group" , cmndCtr );
			RayLight* light = NULL;
			if     ( !strcmp( keyword , "light_point" ) ) light = new RayPointLight();
			else if( !strcmp( keyword , "light_dir"   ) ) light = new RayDirectionalLight();
			else if( !strcmp( keyword , "light_spot"  ) ) light = new RaySpotLight();
			if( !light ) Util::Throw( "[# %d] failed to allocate memory for light" , cmndCtr );
			else light->read( fp );
			lights.push_back( light );
		}

		//Next we try to read out the .key file. We make sure that we have not
		//begun describing the scene graph.
		else if( !strcmp( keyword , "key_file" ) )
		{
			if( inShapes ) Util::Throw( "[# %d] not a valid directive within a group" , cmndCtr );
			std::pair< RayKeyFile , RayKeyData >* keyFileData = new std::pair< RayKeyFile , RayKeyData >();
			if( !keyFileData ) Util::Throw( "[# %d] could not allocate std::pair< RayKeyFile , RayKeyData >" , cmndCtr );
			char keyFileName[BUF_SIZE];
			if( fscanf( fp , " %s" , keyFileName )!=1 ) Util::Throw( "[# %d] failed to read in key file name" , cmndCtr );
			keyFileData->first.read( keyFileName );
			keyFileData->second.set( keyFileData->first , transformType );
			keyFileData->second.setCurrentValue( 0 );
			_keyFileData.push_back( keyFileData );
		}

		//Next we try read in the shader. We make sure that we have
		//not begun describing the scene graph and we make sure that all the
		//file I/O works as it should.
		else if( !strcmp( keyword , "shader" ) )
		{
			if( inShapes ) Util::Throw( "[# %d] not a valid directive within a group" , cmndCtr );
			if( shaderFlag ) Util::Warn( "Parsing Line %d: Ignoring extra shader" , cmndCtr );
			{
				shader = new RayShader();
				if( !shader ) Util::Throw( "[# %d] could not allocate RayShader" , cmndCtr );
				shader->read( fp );
				shaderFlag = true;
			}
		}

		//Next we try read out the scene graph information, obtaining the 
		//transform and either creating a new scene graph or adding a node
		//to the existing one.
		else if( !strcmp( keyword , "group_begin" ) ) inShapes = true , ParseGroup( fp , cmndCtr , root );
		else if( !strcmp( keyword , "joint_begin" ) ) inShapes = true , ParseJoint( fp , cmndCtr , root );
		else if( !strcmp( keyword , "cluster_begin" ) ) inShapes = true , ParseCluster( fp , cmndCtr , root );
		else if( !strcmp( keyword , "shape_triangles_begin" ) ) inShapes = true , ParseTriangleList( fp , cmndCtr , root );
		
		//We check if we have come to the end of a scene-graph group. If we have
		//this is bad because we have not yet opened one
		else if( !strcmp( keyword , "group_end" ) ) Util::Throw( "[# %d] closed more groups than began" , cmndCtr );
		else if( !strcmp( keyword , "joint_end" ) ) Util::Throw( "[# %d] closed more joints than began" , cmndCtr );
		else if( !strcmp( keyword , "cluster_end" ) ) Util::Throw( "[# %d] closed more clusters than began" , cmndCtr );
		else if( !strcmp( keyword , "shape_triangles_end" ) ) Util::Throw( "[# %d] closed more triangle lists than began" , cmndCtr );
		else if( ParseShape( fp , keyword , cmndCtr , root ) ) inShapes = true;
		else Util::Warn( "Ignoring Command %d: No valid #directive... \"%s\"" , cmndCtr , keyword );
	}

	for( int i=0 ; i<_materials.size() ; i++ )
	{
		int tIdx = (int)(size_t)_materials[i]->tex;
		if( tIdx==-1 ) _materials[i]->tex = NULL;
		else if( tIdx>=_textures.size() ) Util::Throw( "[# %d] material specifies a texture out of texture bounds" , cmndCtr );
		else _materials[i]->tex = _textures[tIdx];
	}
}

void RayScene::ParseCluster( FILE* fp , int& cmndCtr , RayGroup* current , std::vector< RayVertex >* vList )
{
	char keyword[BUF_SIZE];
	RayGroup *currNode , *newNode;
	currNode = current;
	newNode = new TrivialRayGroup();
	if( !newNode ) Util::Throw( "[# %d] failed to allocate memory for TrivialRayGroup" , cmndCtr );
	currNode->addShape( newNode );
	currNode = newNode;

	while( fscanf( fp , " #%s" , keyword )==1 )
	{
		cmndCtr++;
		if     ( !strcmp( keyword , "group_begin" ) ) ParseGroup( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "joint_begin" ) ) ParseJoint( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "cluster_begin" ) ) ParseCluster( fp , cmndCtr , currNode , vList );
		else if( !strcmp( keyword , "shape_triangles_begin" ) ) ParseTriangleList( fp , cmndCtr , currNode );

		/* We check if we have come to the end of a scene-graph group and we
		* do the appropriate pointer manipulation, making sure that we are not
		* leaving a group we had not begun */
		else if( !strcmp( keyword , "cluster_end" ) ) return;
		else if( !strcmp( keyword , "group_end") ) Util::Throw( "[# %d] group_end within a cluster group" , cmndCtr );
		else if( !strcmp( keyword , "joint_end") ) Util::Throw( "[# %d] joint_end within a cluster group" , cmndCtr );
		else if( !strcmp( keyword , "shape_triangles_end") ) Util::Throw( "[# %d] shape_triangles_end within a cluster group" , cmndCtr );
		else if( ParseShape( fp , keyword , cmndCtr , currNode , vList ) ){}
		else Util::Warn( "Ignoring Command %d: No valid #directive... \"%s\" in cluster" , cmndCtr , keyword );
	}
	Util::Throw( "[# %d] no cluster_end" , cmndCtr );
}
void RayScene::ParseTriangleList( FILE* fp , int& cmndCtr , RayGroup* current )
{
	bool inShapes = false;
	char keyword[BUF_SIZE];
	int idx;
	RayGroup *currNode;
	TriangleListRayGroup *newNode;
	currNode = current;
	newNode = new TriangleListRayGroup();
	if( !newNode ) Util::Throw( "[# %d] failed to allocate memory for StaticGroup" , cmndCtr );
	newNode->read( fp , &idx );
	if( idx>=_materials.size() ) Util::Throw( "[# %d] shape specifies a material that is out of bounds" , cmndCtr );
	else if( idx<0 ) Util::Throw( "[# %d] you cannot have a negative material index" , cmndCtr );
	else newNode->material = _materials[idx];

	currNode->addShape( newNode );
	currNode = newNode;

	while( fscanf( fp , " #%s" , keyword )==1 )
	{
		cmndCtr++;

		if( !strcmp( keyword , "vertex" ) )
		{
			if( inShapes ) Util::Throw( "[# %d] not a valid directive within a group" , cmndCtr );
			RayVertex rayVertex;
			rayVertex.read( fp );
			rayVertex.index = (int)newNode->vertices.size();
			newNode->vertices.push_back( rayVertex );
		}

		else if( !strcmp( keyword , "group_begin" ) ) Util::Throw( "[# %d] cannot have a static group within a triangle list" , cmndCtr );
		else if( !strcmp( keyword , "joint_begin" ) ) Util::Throw( "[# %d] cannot have a parametrized group within a triangle list" , cmndCtr );
		else if( !strcmp( keyword , "cluster_begin" ) ) inShapes = true , ParseCluster( fp , cmndCtr , currNode , &newNode->vertices );
		else if( !strcmp( keyword , "shape_triangles_begin" ) ) Util::Throw( "[# %d] cannot have a triangle list within a triangle list" , cmndCtr );

		/* We check if we have come to the end of a scene-graph group and we
		* do the appropriate pointer manipulation, making sure that we are not
		* leaving a group we had not begun */
		else if( !strcmp( keyword , "shape_triangles_end" ) ) return;
		else if( !strcmp( keyword , "cluster_end" ) ) Util::Throw( "[# %d] group_end within a triangle list" , cmndCtr );
		else if( !strcmp( keyword , "group_end") ) Util::Throw( "[# %d] group_end within a triangle list" , cmndCtr );
		else if( !strcmp( keyword , "joint_end") ) Util::Throw( "[# %d] joint_end within a triangle list" , cmndCtr );
		else if( ParseShape( fp , keyword , cmndCtr , currNode , &newNode->vertices ) ){}
		else Util::Warn( "Ignoring Command %d: No valid #directive... \"%s\" in triangle list" , cmndCtr , keyword );
	}
	Util::Throw( "[# %d] no shape_triangles_end" , cmndCtr );
}

void RayScene::ParseGroup( FILE* fp , int& cmndCtr , RayGroup* current )
{
	char keyword[BUF_SIZE];
	RayGroup *currNode;
	StaticRayGroup *newNode;
	currNode = current;
	newNode = new StaticRayGroup();
	if( !newNode ) Util::Throw( "[# %d] failed to allocate StaticGroup" , cmndCtr );
	newNode->read( fp );
	currNode->addShape( newNode );
	currNode = newNode;
	
	while( fscanf( fp , " #%s" , keyword )==1 )
	{
		cmndCtr++;
		if     ( !strcmp( keyword , "group_begin" ) ) ParseGroup( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "joint_begin" ) ) ParseJoint( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "cluster_begin" ) ) ParseCluster( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "shape_triangles_begin" ) ) ParseTriangleList( fp , cmndCtr , currNode );
		
		/* We check if we have come to the end of a scene-graph group and we
		* do the appropriate pointer manipulation, making sure that we are not
		* leaving a group we had not begun */
		else if( !strcmp( keyword , "group_end" ) ) return;
		else if( !strcmp( keyword , "joint_end" ) ) Util::Throw( "[# %d] joint_end within a group" , cmndCtr );
		else if( !strcmp( keyword , "cluster_end" ) ) Util::Throw( "[# %d] cluster_end within a static group" , cmndCtr );
		else if( !strcmp( keyword , "shape_triangles_end" ) ) Util::Throw( "[# %d] shape_triangles_end within a static group" , cmndCtr );
		else if( ParseShape( fp , keyword , cmndCtr , currNode ) ){}
		else Util::Warn( "Ignoring Command %d: No valid #directive... \"%s\" in static group" , cmndCtr , keyword );
	}
	Util::Throw( "[# %d] no group_end" , cmndCtr );
}
void RayScene::ParseJoint( FILE* fp , int& cmndCtr , RayGroup* current )
{
	char paramName[BUF_SIZE];
	char keyword[BUF_SIZE];

	ParametrizedRayGroup *newNode;
	RayGroup *currNode;
	int keyFileDataIndex , paramIndex = -1;
	
	currNode = current;
	if( !fscanf( fp , " %s " , paramName ) ) Util::Throw( "[# %d] failed to read parameter name" , cmndCtr );
	for( keyFileDataIndex=0 ; keyFileDataIndex<_keyFileData.size() ; keyFileDataIndex++ )
	{
		paramIndex = _keyFileData[ keyFileDataIndex ]->first.index( paramName );
		if( paramIndex>=0 ) break;
	}
	if( keyFileDataIndex==_keyFileData.size() ) Util::Throw( "[# %d] failed to find joint name in key files" , cmndCtr );
	newNode = _keyFileData[ keyFileDataIndex ]->second.getParametrizedRayGroup( paramIndex );
	if( !newNode ) Util::Throw( "[# %d] failed to link joint node to parameter %d" , cmndCtr , keyFileDataIndex );
	strcpy( newNode->pName , paramName );
	currNode->addShape( newNode );
	currNode = newNode;
	
	while( fscanf( fp , " #%s" , keyword )==1 )
	{
		cmndCtr++;
		if     ( !strcmp( keyword , "group_begin" ) ) ParseGroup( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "joint_begin" ) ) ParseJoint( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "cluster_begin" ) ) ParseCluster( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "shape_triangles_begin" ) ) ParseTriangleList( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "joint_end" ) ) return;
		else if( !strcmp( keyword , "cluster_end" ) ) Util::Throw( "[# %d] cluster_end within a joint" , cmndCtr );
		else if( !strcmp( keyword , "shape_triangles_end" ) ) Util::Throw( "[# %d] shape_triangles_end within a joint" , cmndCtr );
		else if( !strcmp( keyword , "group_end" ) ) Util::Throw( "[# %d] group_end within a joint" , cmndCtr );
		else if( ParseShape( fp , keyword , cmndCtr , currNode ) ){}
		else Util::Warn( "Ignoring Command %d: No valid #directive... \"%s\" in parametrized group" , cmndCtr , keyword );
	}
	Util::Throw( "[# %d] no joint_end" , cmndCtr );
}
bool RayScene::ParseShape( FILE* fp , const char* keyword , int cmndCtr , RayGroup* curr , std::vector< RayVertex >* vList )
{
	RayGroup* current=curr;
	
	/* If we are reading in new files we want them added to the current node
	* and we want to store there information in a temporary scene object we
	* will conveniently forget to free up. */
	if( !vList && !strcmp( keyword , "ray_file_instance" ) )
	{
		int idx;
		if( fscanf( fp , " %d" , &idx)!=1 ) Util::Throw( "[# %d] could not read ray_file index number" , cmndCtr );
		if( idx<0 ) Util::Throw( "[# %d] you cannot have negative ray_file index" , cmndCtr );
		if( idx>=_rayFiles.size() ) Util::Throw( "[# %d] shape specifies a ray_file that is out of bounds" , cmndCtr );
		RayFileInstance* rayFileInstance = new RayFileInstance( _rayFiles[idx] );
		if( !rayFileInstance ) Util::Throw( "[# %d] failed to allocate memory for Shape" , cmndCtr );
		current->addShape( rayFileInstance );
		return true;
	}

	/* We start reading in the shapes. We check that we are inside
	* a group and we call the appropriate instantiator */	

	else if( !vList && !strcmp( keyword , "shape_sphere" ) )
	{
		int idx;
		RaySphere* sphere = new RaySphere();
		if( !sphere ) Util::Throw( "[# %d] failed to allocate memory for Sphere" , cmndCtr );
		sphere->read( fp , &idx );
		if( idx>=_materials.size() ) Util::Throw( "[# %d] shape specifies a material that is out of bounds" , cmndCtr );
		else if( idx<0 ) Util::Throw( "[# %d] you cannot have a negative material index" , cmndCtr );
		else sphere->material = _materials[idx];
		current->addShape( sphere );
		return true;
	}
	
	else if( !vList && !strcmp( keyword , "shape_box" ) )
	{
		int idx;
		RayBox* box = new RayBox();
		if( !box ) Util::Throw( "[# %d] failed to allocate memory for Box" , cmndCtr );
		box->read( fp , &idx );
		if( idx>=_materials.size() ) Util::Throw( "[# %d] shape specifies a material that is out bounds" , cmndCtr );
		else if( idx<0 ) Util::Throw( "[# %d] you cannot have a negative material index" , cmndCtr );
		else box->material = _materials[idx];
		current->addShape( box );
		return true;
	}
    
	else if( !vList && !strcmp( keyword , "shape_cone" ) )
	{
		int idx;
		RayCone* cone = new RayCone();
		if( !cone ) Util::Throw( "[# %d] failed to allocate memory for Cone" , cmndCtr );
		cone->read( fp , &idx );
		if( idx>=_materials.size() ) Util::Throw( "[# %d] shape specifies a material that is out of bounds" , cmndCtr );
		else if( idx<0 ) Util::Throw( "[# %d] you cannot have a negative material index" , cmndCtr );
		else cone->material = _materials[idx];
		current->addShape( cone );
		return true;
	}
	
	else if( !vList && !strcmp( keyword , "shape_cylinder" ) )
	{
		int idx;
		RayCylinder* cylinder = new RayCylinder();
		if( !cylinder ) Util::Throw( "[# %d] failed to allocate memory for Cylinder" , cmndCtr );
		cylinder->read( fp , &idx );
		if( idx>=_materials.size() ) Util::Throw( "[# %d] shape specifies a material that is out of bounds" , cmndCtr );
		else if( idx<0 ) Util::Throw( "[# %d] you cannot have a negative material index" , cmndCtr );
		else cylinder->material = _materials[idx];
		current->addShape( cylinder );
		return true;
	}
	else if( vList && !strcmp( keyword , "shape_triangle" ) )
	{
		RayTriangle* triangle = new RayTriangle();
		if( !triangle ) Util::Throw( "[# %d] failed to allocate memory for Triangle" , cmndCtr );
		triangle->read( fp , *vList );
		current->addShape( triangle );
		return true;
	}
	return false;
}		

void RayScene::setUpOpenGL( int cplx )
{
	if( shader && shader->glslProgram ) shader->glslProgram->setup();
	for( int i=0 ; i<_textures.size() ; i++ ) _textures[i]->setUpOpenGL();
	for( int i=0 ; i<_rayFiles.size() ; i++ ) _rayFiles[i]->scene->setUpOpenGL( cplx );
	group->setUpOpenGL( cplx , 1 );
}

void RayScene::drawOpenGL( void )
{
	camera->drawOpenGL();

#ifndef NO_OPEN_GL
	if( shader && shader->glslProgram ) shader->glslProgram->use();
	else glUseProgram( 0 );

	glEnable( GL_LIGHTING );
#endif // NO_OPEN_GL
	for( int i=0 ; i<lights.size() ; i++ ) lights[i]->drawOpenGL( i , shader ? shader->glslProgram : NULL );
	group->drawOpenGL( -1 , shader ? shader->glslProgram : NULL );
}
void RayScene::setCurrentTime( double t , int curveFit )
{
	for( int i=0 ; i<_keyFileData.size() ; i++ )
	{
		double tt=t/_keyFileData[i]->first.duration;
		tt -= (int)tt;
		_keyFileData[i]->second.setCurrentValue( tt , curveFit );
	}
	for( int i=0 ; i<_rayFiles.size() ; i++ ) _rayFiles[i]->scene->setCurrentTime( t , curveFit );
}

Image32 RayScene::rayTrace( int width , int height , int rLimit , double cLimit )
{
	Image32 img;
	Ray3D ray;
	Pixel32 p;

	img.setSize( width , height );
	ray.position = camera->position;
	for( int i=0 ; i<width ; i++ )
	{
		printf( "           \r" );
		printf( "%3.1f\r" , (float)i/width*100 );
		for( int j=0 ; j<height ; j++ )
		{
			ray = GetRay( camera , i , height-j-1 , width , height );
			Point3D c = getColor( ray , rLimit , Point3D( cLimit , cLimit , cLimit ) );
			p.r = (int)(c[0]*255);
			p.g = (int)(c[1]*255);
			p.b = (int)(c[2]*255);
			img(i,j)=p;
		}
	}
	return img;
}
