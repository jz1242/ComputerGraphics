#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Util/cmdLineParser.h>
#include <Ray/rayScene.h>
#include <Ray/rayWindow.h>


cmdLineParameter< char* > InputRayFile( "in" );
cmdLineParameter< int > Factor( "factor" , RayKeyData::REPRESENTATION_MATRIX+1 );
cmdLineParameter< int > WindowWidth( "width" , 500 );
cmdLineParameter< int > WindowHeight( "height" , 500 );
cmdLineParameter< int > Complexity( "cplx" , 10 );

cmdLineReadable* params[] =
{
	&InputRayFile , &WindowWidth , &WindowHeight , &Complexity , &Factor ,
	NULL
};

void ShowUsage( const char* c )
{
	printf( "Usage %s:\n" , c );
	printf( "\t --%s <input ray File>\n" , InputRayFile.name );
	printf( "\t[--%s <matrix representation>=%d]\n" , Factor.name , Factor.value );
	for( int i=0 ; i<RayKeyData::REPRESENTATION_COUNT ; i++ ) printf( "\t\t%d] %s\n" , i+1 , RayKeyData::RepresentationNames[i] );
	printf( "\t[--%s <window width>=%d]\n" , WindowWidth.name , WindowWidth.value );
	printf( "\t[--%s <window height>=%d]\n" , WindowHeight.name , WindowHeight.value );
	printf( "\t[--%s <complexity>=%d]\n" , Complexity.name , Complexity.value );
}

int main( int argc , char* argv[] )
{
	cmdLineParse( argc-1 , argv+1 , params );
	if( !InputRayFile.set ){ ShowUsage( argv[0] ) ; return EXIT_FAILURE; }

	RayScene scene;
	if( Factor.value<1 || Factor.value>RayKeyData::REPRESENTATION_COUNT )
	{
		printf( "Unsupported factorization type: %d\n" , Factor.value+1 );
		Factor.value = RayKeyData::REPRESENTATION_MATRIX+1;
	}
	try
	{
		scene.read( InputRayFile.value , Factor.value-1 );
		scene.setCurrentTime( 0 );
		RayWindow::RayView( scene , WindowWidth.value , WindowHeight.value , Complexity.value );
	}
	catch( const std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	};
	return EXIT_SUCCESS;
}