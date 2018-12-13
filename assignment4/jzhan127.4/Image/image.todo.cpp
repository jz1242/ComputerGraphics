#include "image.h"
#include <stdlib.h>
#include <math.h>
#include <Util/exceptions.h>

////////////////////////////
// Image processing stuff //
////////////////////////////
Pixel::Pixel( const Pixel32& p )
{
	Util::Throw( "Pixel::Pixel undefined" );
}
Pixel32::Pixel32( const Pixel& p )
{
	Util::Throw( "Pixel32::Pixel32 undefined" );
}

Image32 Image32::addRandomNoise( float noise ) const
{
	Util::Throw( "Image32::addRandomNoise undefined" );
	return Image32();
}
Image32 Image32::brighten( float brightness ) const
{
	Util::Throw( "Image32::brightness undefined" );
	return Image32();
}

Image32 Image32::luminance( void ) const
{
	Util::Throw( "Image32::luminance undefined" );
	return Image32();
}

Image32 Image32::contrast( float contrast ) const
{
	Util::Throw( "Image32::contrast undefined" );
	return Image32();
}

Image32 Image32::saturate( float saturation ) const
{
	Util::Throw( "Image32::saturation undefined" );
	return Image32();
}

Image32 Image32::quantize( int bits ) const
{
	Util::Throw( "Image32::quantize undefined" );
	return Image32();
}

Image32 Image32::randomDither( int bits ) const
{
	Util::Throw( "Image32::randomDither undefined" );
	return Image32();
}
Image32 Image32::orderedDither2X2( int bits ) const
{
	Util::Throw( "Image32::orderedDither2x2 undefined" );
	return Image32();
}

Image32 Image32::floydSteinbergDither( int bits ) const
{
	Util::Throw( "Image32::floydSteinbergDither undefined" );
	return Image32();
}

Image32 Image32::blur3X3( void ) const
{
	Util::Throw( "Image32::blur3X3 undefined" );
	return Image32();
}

Image32 Image32::edgeDetect3X3( void ) const
{
	Util::Throw( "Image32::edgeDetect3X3 undefined" );
	return Image32();
}
Image32 Image32::scaleNearest( float scaleFactor ) const
{
	Util::Throw( "Image32::scaleNearest undefined" );
	return Image32();
}

Image32 Image32::scaleBilinear( float scaleFactor ) const
{
	Util::Throw( "Image32::scaleBilinear undefined" );
	return Image32();
}

Image32 Image32::scaleGaussian( float scaleFactor ) const
{
	Util::Throw( "Image32::scaleGaussian undefined" );
	return Image32();
}

Image32 Image32::rotateNearest( float angle ) const
{
	Util::Throw( "Image32::rotateNearest undefined" );
	return Image32();
}

Image32 Image32::rotateBilinear( float angle ) const
{
	Util::Throw( "Image32::rotateBilinear undefined" );
	return Image32();
}
	
Image32 Image32::rotateGaussian( float angle ) const
{
	Util::Throw( "Image32::rotateGaussian undefined" );
	return Image32();
}

void Image32::setAlpha( const Image32& matte )
{
	Util::Throw( "Image32::setAlpha undefined" );
	return;
}

Image32 Image32::composite( const Image32& overlay ) const
{
	Util::Throw( "Image32::composite undefined" );
	return Image32();
}

Image32 Image32::CrossDissolve( const Image32& source , const Image32& destination , float blendWeight )
{
	Util::Throw( "Image32::CrossDissolve undefined" );
	return Image32();
}
Image32 Image32::warp( const OrientedLineSegmentPairs& olsp ) const
{
	Util::Throw( "Image32::warp undefined" );
	return Image32();
}

Image32 Image32::funFilter( void ) const
{
	Util::Throw( "Image32::funFilter undefined" );
	return Image32();
}
Image32 Image32::crop( int x1 , int y1 , int x2 , int y2 ) const
{
	Util::Throw( "Image32::crop undefined" );
	return Image32();
}

Pixel32 Image32::nearestSample( float x , float y ) const
{
	Util::Throw( "Image32::nearestSample undefined" );
	return Pixel32();
}
Pixel32 Image32::bilinearSample( float x , float y ) const
{
	Util::Throw( "Image32::bilinearSample undefined" );
	return Pixel32();
}
Pixel32 Image32::gaussianSample( float x , float y , float variance , float radius ) const
{
	Util::Throw( "Image32::gaussianSample undefined" );
	return Pixel32();
}
