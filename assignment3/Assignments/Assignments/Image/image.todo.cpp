#include "image.h"
#include <stdlib.h>
#include <math.h>

////////////////////////////
// Image processing stuff //
////////////////////////////
Pixel::Pixel( const Pixel32& p )
{
	throw ImageException( "Pixel::Pixel undefined" );
}
Pixel32::Pixel32( const Pixel& p )
{
	throw ImageException( "Pixel32::Pixel32 undefined" );
}

Image32 Image32::addRandomNoise( float noise ) const
{
	throw ImageException( "Image32::addRandomNoise undefined" );
	return Image32();
}
Image32 Image32::brighten( float brightness ) const
{
	throw ImageException( "Image32::brightness undefined" );
	return Image32();
}

Image32 Image32::luminance( void ) const
{
	throw ImageException( "Image32::luminance undefined" );
	return Image32();
}

Image32 Image32::contrast( float contrast ) const
{
	throw ImageException( "Image32::contrast undefined" );
	return Image32();
}

Image32 Image32::saturate( float saturation ) const
{
	throw ImageException( "Image32::saturation undefined" );
	return Image32();
}

Image32 Image32::quantize( int bits ) const
{
	throw ImageException( "Image32::quantize undefined" );
	return Image32();
}

Image32 Image32::randomDither( int bits ) const
{
	throw ImageException( "Image32::randomDither undefined" );
	return Image32();
}
Image32 Image32::orderedDither2X2( int bits ) const
{
	throw ImageException( "Image32::orderedDither2x2 undefined" );
	return Image32();
}

Image32 Image32::floydSteinbergDither( int bits ) const
{
	throw ImageException( "Image32::floydSteinbergDither undefined" );
	return Image32();
}

Image32 Image32::blur3X3( void ) const
{
	throw ImageException( "Image32::blur3X3 undefined" );
	return Image32();
}

Image32 Image32::edgeDetect3X3( void ) const
{
	throw ImageException( "Image32::edgeDetect3X3 undefined" );
	return Image32();
}
Image32 Image32::scaleNearest( float scaleFactor ) const
{
	throw ImageException( "Image32::scaleNearest undefined" );
	return Image32();
}

Image32 Image32::scaleBilinear( float scaleFactor ) const
{
	throw ImageException( "Image32::scaleBilinear undefined" );
	return Image32();
}

Image32 Image32::scaleGaussian( float scaleFactor ) const
{
	throw ImageException( "Image32::scaleGaussian undefined" );
	return Image32();
}

Image32 Image32::rotateNearest( float angle ) const
{
	throw ImageException( "Image32::rotateNearest undefined" );
	return Image32();
}

Image32 Image32::rotateBilinear( float angle ) const
{
	throw ImageException( "Image32::rotateBilinear undefined" );
	return Image32();
}
	
Image32 Image32::rotateGaussian( float angle ) const
{
	throw ImageException( "Image32::rotateGaussian undefined" );
	return Image32();
}

void Image32::setAlpha( const Image32& matte )
{
	throw ImageException( "Image32::setAlpha undefined" );
	return;
}

Image32 Image32::composite( const Image32& overlay ) const
{
	throw ImageException( "Image32::composite undefined" );
	return Image32();
}

Image32 Image32::CrossDissolve( const Image32& source , const Image32& destination , float blendWeight )
{
	throw ImageException( "Image32::CrossDissolve undefined" );
	return Image32();
}
Image32 Image32::warp( const OrientedLineSegmentPairs& olsp ) const
{
	throw ImageException( "Image32::warp undefined" );
	return Image32();
}

Image32 Image32::funFilter( void ) const
{
	throw ImageException( "Image32::funFilter undefined" );
	return Image32();
}
Image32 Image32::crop( int x1 , int y1 , int x2 , int y2 ) const
{
	throw ImageException( "Image32::crop undefined" );
	return Image32();
}

Pixel32 Image32::nearestSample( float x , float y ) const
{
	throw ImageException( "Image32::nearestSample undefined" );
	return Pixel32();
}
Pixel32 Image32::bilinearSample( float x , float y ) const
{
	throw ImageException( "Image32::bilinearSample undefined" );
	return Pixel32();
}
Pixel32 Image32::gaussianSample( float x , float y , float variance , float radius ) const
{
	throw ImageException( "Image32::gaussianSample undefined" );
	return Pixel32();
}
