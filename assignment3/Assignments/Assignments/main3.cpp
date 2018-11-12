#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Util/cmdLineParser.h>
#include <Ray/rayWindow.h>

cmdLineParameter< char* > InputRayFile( "in" );
cmdLineParameter< int > WindowWidth( "width" , 500 );
cmdLineParameter< int > WindowHeight( "height" , 500 );
cmdLineParameter< int > Complexity( "cplx" , 10 );

cmdLineReadable* params[] =
{
	&InputRayFile , &WindowWidth , &WindowHeight , &Complexity ,
	NULL
};

void ShowUsage( const char* c )
{
	printf( "Usage %s:\n" , c );
	printf( "\t --%s <input ray File>\n" , InputRayFile.name );
	printf( "\t[--%s <window width>=%d]\n" , WindowWidth.name , WindowWidth.value );
	printf( "\t[--%s <window height>=%d]\n" , WindowHeight.name , WindowHeight.value );
	printf( "\t[--%s <complexity>=%d]\n" , Complexity.name , Complexity.value );
}

int main( int argc , char* argv[] )
{
	cmdLineParse( argc-1 , argv+1 , params );
	if( !InputRayFile.set ){ ShowUsage( argv[0] ) ; return EXIT_FAILURE; }

	try
	{
		RayScene scene;
		scene.read( InputRayFile.value );
		RayWindow::RayView( scene , WindowWidth.value , WindowHeight.value , Complexity.value );
	}
	catch( const std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	};

	return EXIT_SUCCESS;
}