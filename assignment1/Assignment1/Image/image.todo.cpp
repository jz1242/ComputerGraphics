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

Pixel::Pixel(const Pixel32& p)
{
	this->r = (float) p.r / 255;
	this->g = (float) p.g / 255;
	this->b = (float) p.b / 255;
	this->a = (float) p.a / 255;

}
Pixel32::Pixel32(const Pixel& p)
{
	this->r = p.r * 255;
	this->g = p.g * 255;
	this->b = p.b * 255;
	this->a = p.a * 255;
}

int Image32::AddRandomNoise(const float& noise,Image32& outputImage) const
{
	//get current object with *this
	outputImage = *this;
	unsigned char rangeNoise = noise * 255;
	int height = this->height();
	int width = this->width();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int amountNoiseR = rand() % (2 * rangeNoise + 1) + (-rangeNoise);
			int amountNoiseG = rand() % (2 * rangeNoise + 1) + (-rangeNoise);
			int amountNoiseB = rand() % (2 * rangeNoise + 1) + (-rangeNoise);
			int amountNoiseA = rand() % (2 * rangeNoise + 1) + (-rangeNoise);
			Pixel32 &pixel = outputImage.pixel(i, j);
			pixel.r = rangeClamp(0, 255, pixel.r + amountNoiseR);
			pixel.g = rangeClamp(0, 255, pixel.g + amountNoiseG);
			pixel.b = rangeClamp(0, 255, pixel.b + amountNoiseB);
			pixel.a = rangeClamp(0, 255, pixel.a + amountNoiseA);

		}
	}
	
	return 1;
}

int Image32::Brighten(const float& brightness,Image32& outputImage) const
{
	outputImage = *this;
	int height = this->height();
	int width = this->width();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage.pixel(i, j);
			pixel.r = rangeClamp(0, 255, pixel.r*brightness);
			pixel.g = rangeClamp(0, 255, pixel.g*brightness);
			pixel.b = rangeClamp(0, 255, pixel.b*brightness);
			pixel.a = rangeClamp(0, 255, pixel.a*brightness);
		}
	}
	return 1;
}

int Image32::Luminance(Image32& outputImage) const
{
	outputImage = *this;
	int height = this->height();
	int width = this->width();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage.pixel(i, j);
			int newVal = rangeClamp(0, 255, pixel.r*0.3 + pixel.g*0.59 + pixel.b*0.11);
			pixel.r = newVal;
			pixel.g = newVal;
			pixel.b = newVal;
		}
	}
	return 1;
}

int Image32::Contrast(const float& contrast,Image32& outputImage) const
{
	outputImage = *this;
	int height = this->height();
	int width = this->width();
	int total = height * width;
	float sum = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage.pixel(i, j);
			sum += pixel.r*0.3 + pixel.g*0.59 + pixel.b*0.11;
		}
	}
	float avg = sum / total;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage.pixel(i, j);
			pixel.r = rangeClamp(0, 255, ((pixel.r - avg)*contrast + avg));
			pixel.g = rangeClamp(0, 255, ((pixel.g - avg)*contrast + avg));
			pixel.b = rangeClamp(0, 255, ((pixel.b - avg)*contrast + avg));
		}
	}
	return 1;
}

int Image32::Saturate(const float& saturation,Image32& outputImage) const
{
	outputImage = *this;
	int height = this->height();
	int width = this->width();
	int total = height * width;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Pixel32 &pixel = outputImage.pixel(i, j);
			float luminance = pixel.r*0.3 + pixel.g*0.59 + pixel.b*0.11;
			pixel.r = rangeClamp(0, 255, ((pixel.r - luminance)*saturation + luminance));
			pixel.g = rangeClamp(0, 255, ((pixel.g - luminance)*saturation + luminance));
			pixel.b = rangeClamp(0, 255, ((pixel.b - luminance)*saturation + luminance));
		}
	}
	return 1;
}

int Image32::Quantize(const int& bits,Image32& outputImage) const
{	
	int val = 1 << (bits);
	float conversion = 1 << (8 - bits);
	outputImage = *this;
	int height = this->height();
	int width = this->width();
	int total = height * width;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height ; j++) {
			Pixel32 &pixel = outputImage.pixel(i, j);

			pixel.r = rangeClamp(0, 255, floor((((float) pixel.r / 256)) * val) * conversion);
			pixel.g = rangeClamp(0, 255, floor((((float) pixel.g / 256)) * val) * conversion);
			pixel.b = rangeClamp(0, 255, floor((((float) pixel.b / 256)) * val) * conversion);

		}
	}
	return 1;
}

int Image32::RandomDither(const int& bits,Image32& outputImage) const
{
	return 0;
}
int Image32::OrderedDither2X2(const int& bits,Image32& outputImage) const
{
	return 0;
}

int Image32::FloydSteinbergDither(const int& bits,Image32& outputImage) const
{
	return 0;
}

int Image32::Blur3X3(Image32& outputImage) const
{
	return 0;
}

int Image32::EdgeDetect3X3(Image32& outputImage) const
{
	return 0;
}
int Image32::ScaleNearest(const float& scaleFactor,Image32& outputImage) const
{
	return 0;
}

int Image32::ScaleBilinear(const float& scaleFactor,Image32& outputImage) const
{
	return 0;
}

int Image32::ScaleGaussian(const float& scaleFactor,Image32& outputImage) const
{
	return 0;
}

int Image32::RotateNearest(const float& angle,Image32& outputImage) const
{
	return 0;
}

int Image32::RotateBilinear(const float& angle,Image32& outputImage) const
{
	return 0;
}
	
int Image32::RotateGaussian(const float& angle,Image32& outputImage) const
{
	return 0;
}


int Image32::SetAlpha(const Image32& matte)
{
	return 0;
}

int Image32::Composite(const Image32& overlay,Image32& outputImage) const
{
	return 0;
}

int Image32::CrossDissolve(const Image32& source,const Image32& destination,const float& blendWeight,Image32& ouputImage)
{
	return 0;
}
int Image32::Warp(const OrientedLineSegmentPairs& olsp,Image32& outputImage) const
{
	return 0;
}

int Image32::FunFilter(Image32& outputImage) const
{
	return 0;
}
int Image32::Crop(const int& x1,const int& y1,const int& x2,const int& y2,Image32& outputImage) const
{
	return 0;
}


Pixel32 Image32::NearestSample(const float& x,const float& y) const
{
	return Pixel32();
}
Pixel32 Image32::BilinearSample(const float& x,const float& y) const
{
	return Pixel32();
}
Pixel32 Image32::GaussianSample(const float& x,const float& y,const float& variance,const float& radius) const
{
	return Pixel32();
}
