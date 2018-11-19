#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Util/cmdLineParser.h>
#include <Ray/rayScene.h>
#if 1
#include <Util/timer.h>
#else
#ifndef WIN32
extern "C"
{
#endif // !WIN32
#include <Util/time.h>
#ifndef WIN32
}
#endif // !WIN32
#endif
#include <chrono>

#undef GLUT_NO_LIB_PRAGMA

cmdLineParameter< char* > InputRayFile( "in" );
cmdLineParameter< char* > OutputImageFile( "out" );
cmdLineParameter< int > ImageWidth( "width" , 500 );
cmdLineParameter< int > ImageHeight( "height" , 500 );
cmdLineParameter< int > RecursionLimit( "rLimit" , 0 );
cmdLineParameter< float > CutOffThreshold( "cutOff" , 0.f );

cmdLineReadable* params[] =
{
	&InputRayFile , &OutputImageFile , &ImageWidth , &ImageHeight , &RecursionLimit , &CutOffThreshold ,
	NULL
};

void ShowUsage( const char* c )
{
	printf( "Usage %s:\n" , c );
	printf( "\t --%s <input ray File>\n" , InputRayFile.name );
	printf( "\t[--%s <output image File>\n" , OutputImageFile.name );
	printf( "\t[--%s <image width>=%d]\n" , ImageWidth.name , ImageWidth.value );
	printf( "\t[--%s <image height>=%d]\n" , ImageHeight.name , ImageHeight.value );
	printf( "\t[--%s <recursion limit>=%d]\n" , RecursionLimit.name , RecursionLimit.value );
	printf( "\t[--%s <cut-off threshold>=%f]\n" , CutOffThreshold.name , CutOffThreshold.value );
}

int main( int argc , char* argv[] )
{
	cmdLineParse( argc-1 , argv+1 , params );
	if( !InputRayFile.set ){ ShowUsage( argv[0] ) ; return EXIT_FAILURE; }

	RayScene scene;

	try
	{
		scene.read( InputRayFile.value );
		scene.camera->aspectRatio = (float)ImageWidth.value / (float)ImageHeight.value;
		scene.group->setBoundingBox();
	
		Timer timer;
		Image32 img = scene.rayTrace( ImageWidth.value , ImageHeight.value , RecursionLimit.value , CutOffThreshold.value );
		printf( "Ray Tracing Time: %f\n" , timer.elapsed() );

		if( OutputImageFile.set ) img.write( OutputImageFile.value );
	}
	catch( const std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	};

	return EXIT_SUCCESS;
}