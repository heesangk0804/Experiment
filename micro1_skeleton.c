#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"


#include <math.h>
#include <stdio.h>
#include <float.h>


void mirror_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);
void grayScale_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);
void sobelFiltering_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);

int main()
{
 
	int height;
	int width;
	int channel;

	char command;

 	printf("Take a picture? (y/n)\n");
 	scanf("%c", &command);

 	if(command == 'n')
 		exit(1);


 	printf("Cheeze !\r\n");
	system("raspistill -w 640 -h 480 -t 2000 -o image.bmp");
 	
 	
 	unsigned char* imgIn = stbi_load("image.bmp", &width, &height, &channel, 3);


	unsigned char* imgOut_mirror = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
	unsigned char* imgOut_grayScale = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
	unsigned char* imgOut_sobelFiltering = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
 	
	mirror_transform(imgIn, height, width, channel, imgOut_mirror);
	grayScale_transform(imgIn, height, width, channel, imgOut_grayScale);
	sobelFiltering_transform(imgOut_grayScale, height, width, channel, imgOut_sobelFiltering);


	stbi_write_bmp("image_mirror.bmp", width, height, channel, imgOut_mirror);
	stbi_write_bmp("image_grayScale.bmp", width, height, channel, imgOut_grayScale);
	stbi_write_bmp("image_sobelFiltering.bmp", width, height, channel, imgOut_sobelFiltering);
	
	stbi_image_free(imgIn);
	free(imgOut_mirror);
	free(imgOut_grayScale);
	free(imgOut_sobelFiltering);
 

	return 0;
}

void mirror_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	int h, w, c;
	for (h = 0; h < height; h++) 
	for (w = 0; w < width; w++) 
	for (c = 0; c < channel; c++) 
		out[h * width * channel + w * channel + c] 
			= in[h * width * channel + (width - w - 1) * channel + c];
}

void grayScale_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	int h, w, c;
	for (h = 0; h < height; h++)
	for (w = 0; w < width; w++) {
		int sum = 0;
		for (c = 0; c < channel; c++) 
			sum += in[h * width * channel + w * channel + c];
		for (c = 0; c < channel; c++)
			out[h * width * channel + w * channel + c] = sum / 3;
	}
}

void sobelFiltering_transform(unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	int h, w, c, x, y;
	int x_sobel[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
	int y_sobel[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
	
	for (h = 1; h < height - 1; h++)
	for (w = 1; w < width - 1; w++) {
		int x_conv = 0;
		int y_conv = 0;
		for (x = 0; x <= 2; x++)
		for (y = 0; y <= 2; y++) {
			x_conv += in[(h + y - 1) * width * channel + (w + x - 1) * channel] * x_sobel[y][x];
			y_conv += in[(h + y - 1) * width * channel + (w + x - 1) * channel] * y_sobel[y][x];
		}
		int sum = abs(x_conv) + abs(y_conv);

		for (c = 0; c < channel; c++)
			out[h * width * channel + w * channel + c] = sum;
	}

	for (h = 0; h < height; h++)
	for (c = 0; c < channel; c++) {
		out[h * width * channel + c] = 0;
		out[h * width * channel + (width - 1) * channel +  c] = 0;
	}
	for (w = 0; w < width; w++)
	for (c = 0; c < channel; c++) {
		out[w * channel + c] = 0;
		out[(height - 1) * width * channel + w * channel + c] = 0;
	}

}
