#include "image.h"
#include <stdlib.h>
#include <math.h>

////////////////////////////
// Image processing stuff //
////////////////////////////
float rotateX(float u, float v, float angle) {
	return u * cos(angle) - v * sin(angle);
}
float rotateY(float u, float v, float angle) {
	return u * sin(angle) + v * cos(angle);
}
float rotateNegX(float u, float v, float angle) {
	return u * cos(-1 * angle) - v * sin(-1 * angle);
}
float rotateNegY(float u, float v, float angle) {
	return (u * sin(-1 * angle) + v * cos(-1 * angle));
}
float checkX(int width, int height, float angle, float rad) {
	if (angle >= 0 && angle < 90) {
		return rotateX(0, height, rad);
	}
	if (angle >= 90 && angle < 180) {
		return rotateX(width, height, rad);
	}
	if (angle >= 180 && angle < 270) {
		return rotateX(width, 0, rad);
	}
	return rotateX(0, 0, rad);
}
float checkY(int width, int height, float angle, float rad) {
	if (angle >= 0 && angle < 90) {
		return rotateY(width, height, rad);
	}
	if (angle >= 90 && angle < 180) {
		return rotateY(width, 0, rad);
	}
	if (angle >= 180 && angle < 270) {
		return rotateY(0, 0, rad);
	}
	return rotateY(0, height, rad);
}
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
int ditherRounding(int x, int y, int value, int mat[2][2], double bits) {
	float dec = (float(value) / 255) * bits;
	float fractional = dec - floor(dec);
	int i = x % 2;
	int j = y % 2;
	if (fractional > (mat[i][j] / (pow(2, 2) + 1))) {
		return ceil(dec)/bits*255;
	}
	else {
		return floor(dec)/bits*255;
	}

}

Pixel::Pixel( const Pixel32& p )
{
	this->r = (float)p.r / 255;
	this->g = (float)p.g / 255;
	this->b = (float)p.b / 255;
	this->a = (float)p.a / 255;
}
Pixel32::Pixel32( const Pixel& p )
{
	this->r = p.r * 255;
	this->g = p.g * 255;
	this->b = p.b * 255;
	this->a = p.a * 255;
}

Image32 Image32::addRandomNoise( float noise ) const
{
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
	Image32 outputImage = *this;
	double val = (1 << (bits)) - 1;

	int mat[2][2] = { 1, 3, 4, 2 };
	int height = this->height();
	int width = this->width();


	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage(i, j);
			pixel.r = ditherRounding(i, j, pixel.r, mat, val);
			pixel.g = ditherRounding(i, j, pixel.g, mat, val);
			pixel.b = ditherRounding(i, j, pixel.b, mat, val);
		}
	}
	return outputImage;
}

Image32 Image32::floydSteinbergDither( int bits ) const
{
	Image32 outputImage = *this;
	int val = (1 << (bits)) - 1;
	int boundary = 256 / val;
	int bucketSize = 256 / (1 << (bits));
	int conversion = 1 << (8 - bits);
	int height = this->height();
	int width = this->width();
	float alpha = 7 / 16.0;
	float beta = 3 / 16.0;
	float gamma = 5 / 16.0;
	float delta = 1 / 16.0;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage(i, j);

			int destR = distance(boundary, pixel.r, bucketSize);
			int destG = distance(boundary, pixel.g, bucketSize);
			int destB = distance(boundary, pixel.b, bucketSize);
			int errorR = pixel.r - destR;
			int errorG = pixel.g - destG;
			int errorB = pixel.b - destB;
			pixel.r = destR;
			pixel.g = destG;
			pixel.b = destB;
			if (j + 1 < height) {
				outputImage(i, j + 1).r = rangeClamp(0, 255, outputImage(i, j + 1).r + (alpha*errorR));
				outputImage(i, j + 1).g = rangeClamp(0, 255, outputImage(i, j + 1).g + (alpha*errorG));
				outputImage(i, j + 1).b = rangeClamp(0, 255, outputImage(i, j + 1).b + (alpha*errorB));
			}
			if (i + 1 < width && j - 1 >= 0) {
				outputImage(i + 1, j - 1).r = rangeClamp(0, 255, outputImage(i + 1, j - 1).r + (beta*errorR));
				outputImage(i + 1, j - 1).g = rangeClamp(0, 255, outputImage(i + 1, j - 1).g + (beta*errorG));
				outputImage(i + 1, j - 1).b = rangeClamp(0, 255, outputImage(i + 1, j - 1).b + (beta*errorB));
			}
			if (i + 1 < height) {
				outputImage(i + 1, j).r = rangeClamp(0, 255, outputImage(i + 1, j).r + (gamma*errorR));
				outputImage(i + 1, j).g = rangeClamp(0, 255, outputImage(i + 1, j).g + (gamma*errorG));
				outputImage(i + 1, j).b = rangeClamp(0, 255, outputImage(i + 1, j).b + (gamma*errorB));
			}
			if (i + 1 < width && j + 1 < height) {
				outputImage(i + 1, j + 1).r = rangeClamp(0, 255, outputImage(i + 1, j + 1).r + (delta*errorR));
				outputImage(i + 1, j + 1).g = rangeClamp(0, 255, outputImage(i + 1, j + 1).g + (delta*errorG));
				outputImage(i + 1, j + 1).b = rangeClamp(0, 255, outputImage(i + 1, j + 1).b + (delta*errorB));
			}
		}
	}
	return outputImage;
}

Image32 Image32::blur3X3( void ) const
{
	Image32 outputImage = *this;
	float mat[3][3] = { 1/16.0, 2/16.0, 1/16.0, 2/16.0, 4/16.0, 2/16.0, 1/16.0, 2/16.0, 1/16.0 };
	int height = this->height();
	int width = this->width();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage(i, j);
			float sumR = pixel.r*mat[1][1];
			float sumG = pixel.g*mat[1][1];
			float sumB = pixel.b*mat[1][1];
			if (i - 1 >= 0 && j - 1 >= 0) {
				sumR += outputImage(i - 1, j - 1).r * mat[0][0];
				sumG += outputImage(i - 1, j - 1).g * mat[0][0];
				sumB += outputImage(i - 1, j - 1).b * mat[0][0];
			}
			if (j - 1 >= 0) {
				sumR += outputImage(i, j - 1).r * mat[0][1];
				sumG += outputImage(i, j - 1).g * mat[0][1];
				sumB += outputImage(i, j - 1).b * mat[0][1];
			}
			if (j - 1 >= 0 && i + 1 < width) {
				sumR += outputImage(i + 1, j - 1).r * mat[0][2];
				sumG += outputImage(i + 1, j - 1).g * mat[0][2];
				sumB += outputImage(i + 1, j - 1).b * mat[0][2];
			}
			if (i - 1 >= 0) {
				sumR += outputImage(i - 1, j).r * mat[1][0];
				sumG += outputImage(i - 1, j).g * mat[1][0];
				sumB += outputImage(i - 1, j).b * mat[1][0];
			}
			
			if (i + 1 < width) {
				sumR += outputImage(i + 1, j).r * mat[1][2];
				sumG += outputImage(i + 1, j).g * mat[1][2];
				sumB += outputImage(i + 1, j).b * mat[1][2];

			}
			if (i - 1 >= 0 && j + 1 < height) {
				sumR += outputImage(i - 1, j + 1).r * mat[2][0];
				sumG += outputImage(i - 1, j + 1).g * mat[2][0];
				sumB += outputImage(i - 1, j + 1).b * mat[2][0];

			}
			if (j + 1 < height) {
				sumR += outputImage(i, j + 1).r * mat[2][1];
				sumG += outputImage(i, j + 1).g * mat[2][1];
				sumB += outputImage(i, j + 1).b * mat[2][1];
			}
			if (i + 1 < width && j + 1 < height) {
				sumR += outputImage(i + 1, j + 1).r * mat[2][2];
				sumG += outputImage(i + 1, j + 1).g * mat[2][2];
				sumB += outputImage(i + 1, j + 1).b * mat[2][2];
			}
			pixel.r = round(sumR);
			pixel.g = round(sumG);
			pixel.b = round(sumB);
		}
	}
	return outputImage;
}

Image32 Image32::edgeDetect3X3( void ) const
{
	Image32 outputImage = *this;
	Image32 temp = *this;
	int mat[3][3] = { -1, -1, -1, -1, 1, -1, -1, -1, -1 };
	int height = this->height();
	int width = this->width();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage(i, j);
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;
			int count = 0;
			if (i - 1 >= 0 && j - 1 >= 0) {
				sumR += temp(i - 1, j - 1).r * mat[0][0];
				sumG += temp(i - 1, j - 1).g * mat[0][0];
				sumB += temp(i - 1, j - 1).b * mat[0][0];
				count++;
			}
			if (j - 1 >= 0) {
				sumR += temp(i, j - 1).r * mat[0][1];
				sumG += temp(i, j - 1).g * mat[0][1];
				sumB += temp(i, j - 1).b * mat[0][1];
				count++;
			}
			if (j - 1 >= 0 && i + 1 < width) {
				sumR += temp(i + 1, j - 1).r * mat[0][2];
				sumG += temp(i + 1, j - 1).g * mat[0][2];
				sumB += temp(i + 1, j - 1).b * mat[0][2];
				count++;
			}
			if (i - 1 >= 0) {
				sumR += temp(i - 1, j).r * mat[1][0];
				sumG += temp(i - 1, j).g * mat[1][0];
				sumB += temp(i - 1, j).b * mat[1][0];
				count++;
			}

			if (i + 1 < width) {
				sumR += temp(i + 1, j).r * mat[1][2];
				sumG += temp(i + 1, j).g * mat[1][2];
				sumB += temp(i + 1, j).b * mat[1][2];
				count++;

			}
			if (i - 1 >= 0 && j + 1 < height) {
				sumR += temp(i - 1, j + 1).r * mat[2][0];
				sumG += temp(i - 1, j + 1).g * mat[2][0];
				sumB += temp(i - 1, j + 1).b * mat[2][0];
				count++;

			}
			if (j + 1 < height) {
				sumR += temp(i, j + 1).r * mat[2][1];
				sumG += temp(i, j + 1).g * mat[2][1];
				sumB += temp(i, j + 1).b * mat[2][1];
				count++;
			}
			if (i + 1 < width && j + 1 < height) {
				sumR += temp(i + 1, j + 1).r * mat[2][2];
				sumG += temp(i + 1, j + 1).g * mat[2][2];
				sumB += temp(i + 1, j + 1).b * mat[2][2];
				count++;
			}
			sumR += pixel.r*mat[1][1]*count;
			sumG += pixel.g*mat[1][1]*count;
			sumB += pixel.b*mat[1][1]*count;
			pixel.r = rangeClamp(0, 255, sumR);
			pixel.g = rangeClamp(0, 255, sumG);
			pixel.b = rangeClamp(0, 255, sumB);

		}
	}
	return outputImage;
}
Image32 Image32::scaleNearest( float scaleFactor ) const
{
	Image32 tmp = *this;
	Image32 outputImage = Image32();
	int height = this->height();
	int width = this->width();
	outputImage.setSize(height*scaleFactor, width*scaleFactor);
	for (int i = 0; i < width*scaleFactor; i++) {
		for (int j = 0; j < height*scaleFactor; j++) {
			float iu = i/scaleFactor;
			float iv = j/scaleFactor;
			Pixel32 &pixel = outputImage(i, j);
			Pixel32 sample = nearestSample(iu, iv);
			pixel.r = sample.r;
			pixel.g = sample.g;
			pixel.b = sample.b;
		}
	}
	return outputImage;
}

Image32 Image32::scaleBilinear( float scaleFactor ) const
{
	Image32 tmp = *this;
	Image32 outputImage = Image32();
	int height = this->height();
	int width = this->width();
	outputImage.setSize(height*scaleFactor, width*scaleFactor);
	for (int i = 0; i < width*scaleFactor; i++) {
		for (int j = 0; j < height*scaleFactor; j++) {
			float u = (i / scaleFactor);
			float v = (j / scaleFactor);
			Pixel32 &pixel = outputImage(i, j); 
			pixel = bilinearSample(u, v);
		}
	}
	return outputImage;
}

Image32 Image32::scaleGaussian( float scaleFactor ) const
{
	Image32 tmp = *this;
	Image32 outputImage = Image32();
	int height = this->height();
	int width = this->width();
	float variance = pow(1 / scaleFactor, 2);
	float radius = (3 * (1 / scaleFactor));
	outputImage.setSize(height*scaleFactor, width*scaleFactor);
	for (int i = 0; i < width*scaleFactor; i++) {
		for (int j = 0; j < height*scaleFactor; j++) {
			float u = (i / scaleFactor);
			float v = (j / scaleFactor);
			Pixel32 temp = gaussianSample(u, v, variance, radius);
			Pixel32 &pixel = outputImage(i, j);
			pixel.r = temp.r;
			pixel.g = temp.g;
			pixel.b = temp.b;
		}
	}
	return outputImage;
}
Image32 Image32::rotateNearest( float angle ) const
{
	float rad = angle * PI / 180.0;
	int orig_width = this->width();
	int orig_height = this->height();
	int width = orig_width * abs(cos(rad)) + orig_height * abs(sin(rad)); 
	int height = orig_width * abs(sin(rad)) + orig_height * abs(cos(rad));
	Image32 outputImage = Image32();
	outputImage.setSize(width, height);
	float offsetX = checkX(orig_width, orig_height, angle, rad);
	float offsetY = checkY(orig_width, orig_height, angle, rad);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float x = i + offsetX;
			float y = offsetY - j;
			float u = rotateNegX(x, y, rad);
			float v = orig_height - rotateNegY(x, y, rad);
			if (u >= 0 && u < orig_width && v >= 0 && v < orig_height) {
				Pixel32 &pixel = outputImage(i, j);
				pixel = nearestSample(u, v);
			}
		}
	}

	return outputImage;
}

Image32 Image32::rotateBilinear( float angle ) const
{
	float rad = angle * PI / 180.0;
	int orig_width = this->width();
	int orig_height = this->height();
	int width = orig_width * abs(cos(rad)) + orig_height * abs(sin(rad));
	int height = orig_width * abs(sin(rad)) + orig_height * abs(cos(rad));
	Image32 outputImage = Image32();
	outputImage.setSize(width, height);
	float offsetX = checkX(orig_width, orig_height, angle, rad);
	float offsetY = checkY(orig_width, orig_height, angle, rad);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float x = i + offsetX;
			float y = offsetY - j;
			float u = rotateNegX(x, y, rad);
			float v = orig_height - rotateNegY(x, y, rad);
			if (u >= 0 && u < orig_width && v >= 0 && v < orig_height) {
				Pixel32 &pixel = outputImage(i, j);
				pixel = bilinearSample(u, v);
			}
		}
	}

	return outputImage;
}
	
Image32 Image32::rotateGaussian( float angle ) const
{
	float variance = 1;
	float radius = 3;
	float rad = angle * PI / 180.0;
	int orig_width = this->width();
	int orig_height = this->height();
	int width = orig_width * abs(cos(rad)) + orig_height * abs(sin(rad));
	int height = orig_width * abs(sin(rad)) + orig_height * abs(cos(rad));
	Image32 outputImage = Image32();
	outputImage.setSize(width, height);
	float offsetX = checkX(orig_width, orig_height, angle, rad);
	float offsetY = checkY(orig_width, orig_height, angle, rad);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float x = i + offsetX;
			float y = offsetY - j;
			float u = rotateNegX(x, y, rad);
			float v = orig_height - rotateNegY(x, y, rad);
			if (u >= 0 && u < orig_width && v >= 0 && v < orig_height) {
				Pixel32 &pixel = outputImage(i, j);
				pixel = gaussianSample(u, v, variance, radius);
			}
		}
	}

	return outputImage;
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
	throw ImageException("Image32::warp undefined");
	return Image32();
}
Image32 Image32::crop( int x1 , int y1 , int x2 , int y2 ) const
{
	int width = abs(x2 - x1);
	int height = abs(y2 - y1);
	Image32 output = Image32();
	Image32 orig = *this;
	output.setSize(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height ; j++) {
			output(i, j) = (*this)(i + x1, j + y1);
		}
	}
	return output;
}

Pixel32 Image32::nearestSample( float x , float y ) const
{
	int iu = (int)floor(x);
	int iv = (int)floor(y);
	Image32 orig = *this;
	Pixel32 &pixel = orig(iu, iv);
	return pixel;
}
Pixel32 Image32::bilinearSample( float x , float y ) const
{
	Image32 tmp = *this;
	int height = tmp.height();
	int width = tmp.width();
	int u1 = floor(x);
	int v1 = floor(y);
	int u2 = u1 + 1;
	int v2 = v1 + 1;
	float du = x - u1;
	float ar = 0;
	float ag = 0;
	float ab = 0;
	float br = 0;
	float bg = 0;
	float bb = 0;
	ar = tmp(u1 % height, v1 % width).r*(1 - du) + tmp(u2 % height, v1 % width).r*(du);
	ag = tmp(u1 % height, v1 % width).g*(1 - du) + tmp(u2 % height, v1 % width).g*(du);
	ab = tmp(u1 % height, v1 % width).b*(1 - du) + tmp(u2 % height, v1 % width).b*(du);
	br = tmp(u1 % height, v2 % width).r*(1 - du) + tmp(u2 % height, v2 % width).r*(du);
	bg = tmp(u1 % height, v2 % width).g*(1 - du) + tmp(u2 % height, v2 % width).g*(du);
	bb = tmp(u1 % height, v2 % width).b*(1 - du) + tmp(u2 % height, v2 % width).b*(du);
	float dv = y - v1;
	Pixel32 pixel;
	pixel.r = ar * (1 - dv) + (br*dv);
	pixel.g = ag * (1 - dv) + (bg*dv);
	pixel.b = ab * (1 - dv) + (bb*dv);
	return pixel;
}
Pixel32 Image32::gaussianSample( float x , float y , float variance , float radius ) const
{
	Image32 orig = *this;
	Pixel32 &pixel = orig(x, y);
	Pixel32 output;
	int lower_row = x - radius >= 0 ? x - radius : 0;
	int upper_row = x + radius < orig.width() ? x + radius : orig.width();
	int lower_col = y - radius >= 0 ? y - radius : 0;
	int upper_col = y + radius < orig.height() ? y + radius : orig.height();
	float total = 0;
	float r = 0;
	float g = 0;
	float b = 0;
	for (int i = lower_row; i < upper_row; i++) {
		for (int j = lower_col; j < upper_col; j++) {
			Pixel32 &pixel = orig(i, j);
			float di = (i - x);
			float dj = (j - y);
			float gaussian = exp((-(pow(di, 2) + pow(dj, 2))) / (2 * variance));
			total += gaussian;
			r += pixel.r * gaussian;
			g += pixel.g * gaussian;
			b += pixel.b * gaussian;

		}
	}
	output.r = r / total;
	output.g = g / total;
	output.b = b / total;


	return output;
}


