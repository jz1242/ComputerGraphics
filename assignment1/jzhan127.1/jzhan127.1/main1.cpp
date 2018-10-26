#include <stdio.h>
#include <stdlib.h>
#include "Image/bmp.h"
#include "Image/jpeg.h"
#include "Image/image.h"
#include "Util/cmdLineParser.h"

cmdLineParameter< char* > Input( "in" );
cmdLineParameter< char* > Output( "out" );
cmdLineParameterArray< char* , 2 > Composite( "composite" );
cmdLineParameterArray< char* , 3 > BeierNeelyMorph( "bnMorph" );
cmdLineParameterArray< int , 4 > Crop( "crop" );
cmdLineParameter< float > Noisify( "noisify" , 0.f );
cmdLineParameter< float > Brighten( "brighten" , 1.f  );
cmdLineParameter< float > Contrast( "contrast" , 1.f );
cmdLineParameter< float > Saturate( "saturate" , 1.f );
cmdLineParameter< float > ScaleNearest( "scaleNearest" , 1.f );
cmdLineParameter< float > ScaleBilinear( "scaleBilinear" , 1.f );
cmdLineParameter< float > ScaleGaussian( "scaleGaussian" , 1.f );
cmdLineParameter< float > RotateNearest( "rotateNearest" , 0.f );
cmdLineParameter< float > RotateBilinear( "rotateBilinear" , 0.f );
cmdLineParameter< float > RotateGaussian( "rotateGaussian" , 0.f );
cmdLineParameter< int > Quantize( "quantize" , 8 );
cmdLineParameter< int > RandomDither( "rDither" , 8 );
cmdLineParameter< int > OrderedDither2X2( "oDither2x2" , 8 );
cmdLineParameter< int > FloydSteinbergDither( "fsDither" , 8 );
cmdLineReadable Gray( "gray" );
cmdLineReadable Blur3X3( "blur3x3" );
cmdLineReadable Edges3X3( "edges3x3" );
cmdLineReadable Fun( "fun" );

cmdLineReadable* params[] =
{
	&Input , &Output , &Composite , &BeierNeelyMorph , &Crop , &Noisify , &Brighten , &Contrast , &Saturate ,
	&ScaleNearest , &ScaleBilinear , &ScaleGaussian , &RotateNearest , &RotateBilinear , &RotateGaussian ,
	&Quantize , &RandomDither , &OrderedDither2X2 , &FloydSteinbergDither , &Gray , &Blur3X3 , &Edges3X3 , &Fun ,
	NULL
};

void ShowUsage( char* ex )
{
	printf( "Usage %s:\n" , ex );
	printf( "\t --%s <input image>\n" , Input.name );
	printf( "\t[--%s <output image>]\n" , Output.name );
	printf( "\t[--%s <size of noise>=%f]\n" , Noisify.name , Noisify.value );
	printf( "\t[--%s <brightening factor>=%f]\n" , Brighten.name , Brighten.value );
	printf( "\t[--%s <contrast factor>=%f]\n" , Contrast.name , Contrast.value );
	printf( "\t[--%s <saturation factor>=%f]\n" , Saturate.name , Saturate.value );
	printf( "\t[--%s <bits per channel>=%d]\n" , Quantize.name , Quantize.value );
	printf( "\t[--%s <bits per channel with random dithering>=%d]\n" , RandomDither.name , RandomDither.value );
	printf( "\t[--%s <bits per channel with ordered dithering>=%d]\n" , OrderedDither2X2.name , OrderedDither2X2.value );
	printf( "\t[--%s <bits per channel with Floyd-Steinberg dithering>=%d]\n" , FloydSteinbergDither.name , FloydSteinbergDither.value );
	printf( "\t[--%s <overlay image> <matte image>]\n" , Composite.name );
	printf( "\t[--%s <destination image> <line segment pair list> <time step>]\n" , BeierNeelyMorph.name );
	printf( "\t[--%s <x1> <y1> <x2> <y2>]\n" , Crop.name );
	printf( "\t[--%s <scale factor>=%f]\n" , ScaleNearest.name , ScaleNearest.value );
	printf( "\t[--%s <scale factor>=%f]\n" , ScaleBilinear.name , ScaleBilinear.value );
	printf( "\t[--%s <scale factor>=%f]\n" , ScaleGaussian.name , ScaleGaussian.value );
	printf( "\t[--%s <angle (in degrees)>=%f]\n" , RotateNearest.name , RotateNearest.value );
	printf( "\t[--%s <angle (in degrees)>=%f]\n" , RotateBilinear.name , RotateBilinear.value );
	printf( "\t[--%s <angle (in degrees)>=%f]\n" , RotateGaussian.name , RotateGaussian.value );
	printf( "\t[--%s]\n" , Blur3X3.name );
	printf( "\t[--%s]\n" , Edges3X3.name );
	printf( "\t[--%s]\n" , Fun.name );
	printf( "\t[--%s]\n" , Gray.name );
}

int main( int argc , char* argv[] )
{
	cmdLineParse( argc-1 , argv+1 , params );
	if( !Input.set ) { ShowUsage( argv[0] ) ; return EXIT_FAILURE; }

	// Try to read in the input image
	Image32 image;
	image.read( Input.value );
	printf( "Input dimensions: %d x %d\n" , image.width() , image.height() );

	// Filter the image
	if( Noisify.set )              image = image.addRandomNoise( Noisify.value );
	if( Brighten.set )             image = image.brighten( Brighten.value );
	if( Gray.set )                 image = image.luminance();
	if( Contrast.set )             image = image.contrast( Contrast.value );
	if( Saturate.set )             image = image.saturate( Saturate.value );
	if( Quantize.set )             image = image.quantize( Quantize.value );
	if( RandomDither.set )         image = image.randomDither( RandomDither.value );
	if( OrderedDither2X2.set )     image = image.orderedDither2X2( OrderedDither2X2.value );
	if( FloydSteinbergDither.set ) image = image.floydSteinbergDither( FloydSteinbergDither.value );

	if( Composite.set )
	{
		Image32 overlay , matte;
		// Read in the target image
		overlay.read( Composite.values[0] );
		// Read in the matte image
		matte.read( Composite.values[1] );
		// Set the alpha value of the overlay image using the values of the matte image
		overlay.setAlpha( matte );
		// Perform the compositing
		image = image.composite( overlay );
	}
	if( Blur3X3.set )  image = image.blur3X3();
	if( Edges3X3.set ) image = image.edgeDetect3X3();
	if( ScaleNearest.set )  image = image.scaleNearest ( ScaleNearest.value  );
	if( ScaleBilinear.set ) image = image.scaleBilinear( ScaleBilinear.value );
	if( ScaleGaussian.set ) image = image.scaleGaussian( ScaleGaussian.value );
	if( RotateNearest.set )  image = image.rotateNearest ( RotateNearest.value  );
	if( RotateBilinear.set ) image = image.rotateBilinear( RotateBilinear.value );
	if( RotateGaussian.set ) image = image.rotateGaussian( RotateGaussian.value );

	if( Fun.set ) image = image.funFilter();
	if( Crop.set ) image = image.crop( Crop.values[0] , Crop.values[1] , Crop.values[2] , Crop.values[3] );

	if( BeierNeelyMorph.set )
	{
		float timeStep = (float)atof( BeierNeelyMorph.values[2] );
		Image32 dest;
		OrientedLineSegmentPairs olsp;

		// Read the destination image
		dest.read( BeierNeelyMorph.values[0] );
		// Read in the list of corresponding line segments
		olsp.read( BeierNeelyMorph.values[1] );
		image = Image32::BeierNeelyMorph( image , dest , olsp , timeStep );
	}

	printf( "Output dimensions: %d x %d\n" , image.width() , image.height() );

	// Try to write out the output image
	if( Output.set ) image.write( Output.value );
	return EXIT_SUCCESS;
}
