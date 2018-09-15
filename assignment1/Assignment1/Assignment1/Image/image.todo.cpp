#include "image.h"
#include <stdlib.h>
#include <math.h>

////////////////////////////
// Image processing stuff //
////////////////////////////
int rangeClamp(int low, int high, int val) {
	if (val < low) {
		return low;
	}
	if (val > high) {
		return high;
	}
	return val;
}
int distance(int boundary, int value, int bucketSize) {
	int len = 256 / boundary;

	int* boundaryValues = new int[len];
	int count = 1;
	int ind = 1;
	boundaryValues[0] = 0;
	while (boundary * count < 255) {
		boundaryValues[count] = boundary * count;
		count++;
	}
	boundaryValues[count] = 255;

	int pick = 0;
	for (int i = 1; i <= count + 1; i++) {
		if (value <= bucketSize * i) {
			pick = (i - 1);
			break;
		}
	}
	int toRet = boundaryValues[pick];
	delete[] boundaryValues;
	return toRet;
}
Pixel::Pixel( const Pixel32& p )
{
	this->r = (float)p.r / 255;
	this->g = (float)p.g / 255;
	this->b = (float)p.b / 255;
	this->a = (float)p.a / 255;
	throw ImageException( "Pixel::Pixel undefined" );
}
Pixel32::Pixel32( const Pixel& p )
{
	this->r = p.r * 255;
	this->g = p.g * 255;
	this->b = p.b * 255;
	this->a = p.a * 255;
	throw ImageException( "Pixel32::Pixel32 undefined" );
}

Image32 Image32::addRandomNoise( float noise ) const
{
	//get current object with *this
	Image32 outputImage = *this;
	unsigned char rangeNoise = noise * 255;
	int height = this->height();
	int width = this->width();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int amountNoiseR = rand() % (2 * rangeNoise + 1) + (-rangeNoise);
			int amountNoiseG = rand() % (2 * rangeNoise + 1) + (-rangeNoise);
			int amountNoiseB = rand() % (2 * rangeNoise + 1) + (-rangeNoise);
			int amountNoiseA = rand() % (2 * rangeNoise + 1) + (-rangeNoise);
			Pixel32 &pixel = outputImage(i, j);
			pixel.r = rangeClamp(0, 255, pixel.r + amountNoiseR);
			pixel.g = rangeClamp(0, 255, pixel.g + amountNoiseG);
			pixel.b = rangeClamp(0, 255, pixel.b + amountNoiseB);
			pixel.a = rangeClamp(0, 255, pixel.a + amountNoiseA);

		}
	}

	return outputImage;
}
Image32 Image32::brighten( float brightness ) const
{
	Image32 outputImage = *this;
	int height = this->height();
	int width = this->width();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage(i, j);
			pixel.r = rangeClamp(0, 255, pixel.r*brightness);
			pixel.g = rangeClamp(0, 255, pixel.g*brightness);
			pixel.b = rangeClamp(0, 255, pixel.b*brightness);
			pixel.a = rangeClamp(0, 255, pixel.a*brightness);
		}
	}
	return outputImage;
}

Image32 Image32::luminance( void ) const
{
	Image32 outputImage = *this;
	int height = this->height();
	int width = this->width();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage(i, j);
			int newVal = rangeClamp(0, 255, pixel.r*0.3 + pixel.g*0.59 + pixel.b*0.11);
			pixel.r = newVal;
			pixel.g = newVal;
			pixel.b = newVal;
		}
	}
	return outputImage;
}

Image32 Image32::contrast( float contrast ) const
{
	Image32 outputImage = *this;
	int height = this->height();
	int width = this->width();
	int total = height * width;
	float sum = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage(i, j);
			sum += pixel.r*0.3 + pixel.g*0.59 + pixel.b*0.11;
		}
	}
	float avg = sum / total;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage(i, j);
			pixel.r = rangeClamp(0, 255, ((pixel.r - avg)*contrast + avg));
			pixel.g = rangeClamp(0, 255, ((pixel.g - avg)*contrast + avg));
			pixel.b = rangeClamp(0, 255, ((pixel.b - avg)*contrast + avg));
		}
	}
	return outputImage;
}

Image32 Image32::saturate( float saturation ) const
{
	Image32 outputImage = *this;
	int height = this->height();
	int width = this->width();
	int total = height * width;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage(i, j);
			float luminance = pixel.r*0.3 + pixel.g*0.59 + pixel.b*0.11;
			pixel.r = rangeClamp(0, 255, ((pixel.r - luminance)*saturation + luminance));
			pixel.g = rangeClamp(0, 255, ((pixel.g - luminance)*saturation + luminance));
			pixel.b = rangeClamp(0, 255, ((pixel.b - luminance)*saturation + luminance));
		}
	}
	return outputImage;
}

Image32 Image32::quantize( int bits ) const
{
	int val = (1 << (bits)) - 1;
	int boundary = 256 / val;
	int bucketSize = 256 / (1 << (bits));
	int conversion = 1 << (8 - bits);
	Image32 outputImage = *this;
	int height = this->height();
	int width = this->width();
	int total = height * width;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage(i, j);
			pixel.r = distance(boundary, pixel.r, bucketSize);
			pixel.g = distance(boundary, pixel.g, bucketSize);
			pixel.b = distance(boundary, pixel.b, bucketSize);
		}
	}
	return outputImage;
}

Image32 Image32::randomDither( int bits ) const
{
	
	Image32 outputImage = this->addRandomNoise(1.0 / (1 << bits)).quantize(bits);
	return outputImage;
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
