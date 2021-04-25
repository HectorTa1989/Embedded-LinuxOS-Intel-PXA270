#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct _bitmapFile {
	char title[2];
	int filesize;
	int zerobytes;
	int bmpoffset;
	int headersize;
	int width;
	int height;
	short nplanes;
	short bitsperpixel;
	int compression;
	int bmpdatasize;
	int pixelsmeter1;
	int pixelsmeter2;
	int palettesize;
	int important;
	// memory
	char * bitmapData;
	int retaincount;
	// extra
	int linepadding;
};

typedef struct _bitmapFile bitmapFile;
typedef bitmapFile* ANBitmapFileRef;
typedef int ANPixel;

// creation

ANBitmapFileRef ANBitmapFileCreate (int width, int height);
ANBitmapFileRef ANBitmapFileCreateCopy (ANBitmapFileRef bmp);
ANBitmapFileRef ANBitmapFileCreateWithFile (char * file);
void ANBitmapFileGetSize (ANBitmapFileRef bmp, int * width, int * height);

// pixels

void ANBitmapFileSetPixel (ANBitmapFileRef bmp, int x, int y, ANPixel pixel);
void ANPixelGetComponents (ANPixel pixel, int * r, int * g, int * b, int * a);
ANPixel ANBitmapFileGetPixel (ANBitmapFileRef bmp, int x, int y);
ANPixel ANPixelWithComponents (int red, int green, int blue, int alpha);

// saving

void ANBitmapFileWriteToFile (ANBitmapFileRef bmp, char * path);

// deletion

ANBitmapFileRef ANBitmapFileRetain (ANBitmapFileRef bmp);
void ANBitmapFileRelease (ANBitmapFileRef bmp);