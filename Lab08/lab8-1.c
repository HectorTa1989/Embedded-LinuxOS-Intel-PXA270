#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/fcntl.h>
#include <string.h>
#include "qdbmp.h"
//#include"asm-arm/arch-pxa/lib/creator_pxa270_cmos.h"
#define BMP_C__
#define RGB(r, g, b) ((((r)&0xFF)<<16) + (((g)&0xFF)<<8) + ((b)&0xFF))
#define CEIL4(x) ((((x)+3)/4)*4)
#define BMP_PALETTE_SIZE	( 256 * 4 )
//#define BMP_PALETTE_SIZE	( 16777216 * 4 )
//int savebmp("lab8_1.bmp");
int savebmp(char *fname);
unsigned int  width = 160,height = 120, depth = 24;
FILE *fp_t = NULL;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
//typedef unsigned short USHORT;
typedef struct tagBmpHeader {
//	char  type[2];       // = "BM", it is matipulated separately to make the size of this structure the multiple of 4, 
/*	USHORT Magic;
	DWORD sizeFile;      // = offbits + sizeImage 
//	DWORD reserved;      // == 0 
	USHORT		Reserved1;			// Reserved 
	USHORT		Reserved2;			// Reserved 
	DWORD offbits;       // offset from start of file = 54 + size of palette
	DWORD sizeStruct;    // 40 
	DWORD width, height;
	WORD  planes;        // 1 
	WORD  bitCount;      // bits of each pixel, 256color it is 8, 24 color it is 24 
	DWORD compression;   // 0 
	DWORD sizeImage;     // (width+?)(till multiple of 4) * height£¬in bytes 
	DWORD xPelsPerMeter; // resolution in mspaint 
	DWORD yPelsPerMeter;
	DWORD colorUsed;     // if use all, it is 0
	DWORD colorImportant;*/

	USHORT		Magic;				/* Magic identifier: "BM" */
	UINT		FileSize;			/* Size of the BMP file in bytes */
	USHORT		Reserved1;			/* Reserved */
	USHORT		Reserved2;			/* Reserved */
	UINT		DataOffset;			/* Offset of image data relative to the file's start */
	UINT		HeaderSize;			/* Size of the header in bytes */
	UINT		Width;				/* Bitmap's width */
	UINT		Height;				/* Bitmap's height */
	USHORT		Planes;				/* Number of color planes in the bitmap */
	USHORT		BitsPerPixel;		/* Number of bits per pixel */
	UINT		CompressionType;	/* Compression type */
	UINT		ImageDataSize;		/* Size of uncompressed image's data */
	UINT		HPixelsPerMeter;	/* Horizontal resolution (pixels per meter) */
	UINT		VPixelsPerMeter;	/* Vertical resolution (pixels per meter) */
	UINT		ColorsUsed;			/* Number of color indexes in the color table that are actually used by the bitmap */
	UINT		ColorsRequired;		/* Number of color indexes that are required for displaying the bitmap */
}Header;
//Header head = {0};

/*typedef struct tagBitmap{
	size_t width;
	size_t height;
	size_t size;
	unsigned char *data;
}Bitmap;
Bitmap mbmp = {0};
int    misopen = 0;
size_t mcolor = 0;*/

	
/* create a new bitmap file */
/*int newbmp(int width, int height){
	if (width <= 0 || height <= 0) {
		printf("Width and height should be positve.\n");
		return 1;
	}

	if (misopen) closebmp();
	// fill bmp struct 
	mbmp.width = (size_t)width;
	mbmp.height = (size_t)height;
	mbmp.size = CEIL4(mbmp.width * 3) * mbmp.height;

	if ((mbmp.data = malloc(mbmp.size)) == NULL){
		printf("Error: alloc fail!");
		return 1;
	}
	memset(mbmp.data, 0xFF, mbmp.size); //make background color white 

	misopen = 1;

	return 0;
}*/
// load a .bmp file to a Bitmap struct, on error return 0  
/*int openbmp(char *fname)
{
	FILE *fp;
	BmpHeader head = {0};

	if ((fp = fopen(fname, "rb")) == NULL){
		printf("Error: can't open bmp file \"%s\".\n", fname);
		return 1;
	}

	// read head, and check its format 
	if (fgetc(fp) != 'B' || fgetc(fp) != 'M') {
		printf("Error: bmp file \"%s\" format error.", fname);
		goto ERR_EXIT;
	}

	// read its infomation 
	fread(&head, sizeof head, 1, fp);
	if (head.sizeStruct != 40 || head.reserved != 0){
		printf("Error: bmp file \"%s\" format error.", fname);
		goto ERR_EXIT;
	}

	// check format 
	if (head.bitCount != 24){
		printf("Sorry: bmp file \"%s\" bit count != 24", fname);
		goto ERR_EXIT;
	}

	// close old bitmap 
	if (misopen) closebmp();

	// fill the bitmap struct 
	mbmp.width = head.width;
	mbmp.height = head.height;
	mbmp.size = head.sizeImage;
	if (mbmp.size != CEIL4(mbmp.width * 3) * mbmp.height){ // check size 
		printf("Error: bmp file \"%s\" size do not match!\n", fname);
		goto ERR_EXIT;
	}

	// allocate memory 
	if ((mbmp.data = realloc(mbmp.data, head.sizeImage)) == NULL){
		printf("Error: alloc fail!");
		goto ERR_EXIT;
	}
	// read data into memory 
	if (fread(mbmp.data, 1, mbmp.size, fp) != mbmp.size){
		printf("Error: read data fail!");
		goto ERR_EXIT;
	}

	misopen = 1;
	fclose(fp);
	return 0;

ERR_EXIT:
	misopen = 0;
	fclose(fp);
	return 1;
}	*/

/*int savebmp(char *fname)
{
//	FILE *fp;
	Head = {0, 0, 54, 40, 0, 0, 1, 24, 0, 0}; // BMP file header 

	if ((fp_t = fopen("lab8_1.bmp", "wb")) == NULL) {
		printf("Error: can't save to BMP file \"%s\".\n", "lab8_1.bmp");
		return 1;
	}

	fputc('B', fp_t); fputc('M', fp_t); // write type 
	// fill BMP file header 
	Head.width = mbmp.width;
	Head.height = mbmp.height;
	Head.sizeImage = mbmp.size;
	Head.sizeFile = mbmp.size + head.offbits;
	fwrite(&Head, sizeof Head, 1, fp_t); // write header 
	if (fwrite(mbmp.data, 1, mbmp.size, fp_t) != mbmp.size) {
		fclose(fp_t);
		return 1; // write bitmap infomation 
	}

	fclose(fp_t);
	return 0;
}	*/
////////////////////////      /////////////////////////
int main()
{
	int terminate = 1;
	int dwTotalReadSize,dwImageSize,nRead;
	int count;
	int fd_cmos;
//	cmos_info_t CMOSInfo;
	unsigned char *pImageBuffer = NULL;
	
	dwImageSize = 160*120;
	pImageBuffer = malloc(dwImageSize);
/*	fd_cmos = open("/dev/cmos",O_RDWR);
	CMOSInfo.ImageSizeMode = CMOS_SIZE_MODE_160_120;
	CMOSInfo.HighRef = 120;
	ioctl(fd_cmos,CMOS_PARAMETER,&CMOSInfo);
	ioctl(fd_cmos,CMOS_ON,&CMOSInfo);
	
	//while(terminate)
	//{
		if(ioctl(fd_cmos,CMOS_GET_STATUS,&CMOSInfo)>=0)
		{
			dwTotalReadSize = 0;
			count = dwImageSize;
			do
			{
				if(count+dwTotalReadSize>dwImageSize)
					count = dwImageSize - dwTotalReadSize;
				if((nRead = read(fd_cmos,pImageBuffer+dwTotalReadSize,count))>0)
					dwTotalReadSize += nRead;
				else if(nRead == 0)
					break;
			}
			while(dwTotalReadSize<dwImageSize);
		}
*/	
		//data process
		
//		unsigned int  width = 160,height = 120;
		unsigned int  x,y;
		unsigned char R, G, B;
		unsigned char *image_t = NULL;
		unsigned int  x_avg;            
		unsigned int  y_t; 
//	  	FILE          *fp_t = NULL;
	Header.Magic				= 0x4D42;
	Header.Reserved1			= 0;
	Header.Reserved2			= 0;
	Header.HeaderSize			= 40;
	Header.Planes				= 1;
	Header.CompressionType		= 0;
	Header.HPixelsPerMeter		= 0;
	Header.VPixelsPerMeter		= 0;
	Header.ColorsUsed			= 0;
	Header.ColorsRequired		= 0;
	Header.Width				= width;
	Header.Height				= height;
	Header.BitsPerPixel		= depth;
	Header.ImageDataSize		= width* height* depth;        // E.g: a 4×2 pixel bitmap, with 32 bits/pixel (4 bytes/pixel) encoding would have Size of the raw bitmap data= 4*2*4= 32 bytes 
	Header.FileSize			= Header.ImageDataSize + 54 + ( depth == 24 ? BMP_PALETTE_SIZE : 0 );
	Header.DataOffset			= 54 + ( depth == 24 ? BMP_PALETTE_SIZE : 0 );

/*	  
		unsigned char header[54] = 
		{
	  		0x42,        // identity : B
			0x4d,        // identity : M
	    		0, 0, 0, 0,  // file size
	   		0, 0,        // reserved1
	    		0, 0,        // reserved2
			54, 0, 0, 0, // RGB data offset
	    		40, 0, 0, 0, // struct BITMAPINFOHEADER size
	    		0, 0, 0, 0,  // bmp width
	    		0, 0, 0, 0,  // bmp height
	    		1, 0,        // planes
	    		24, 0,       // bit per pixel
	    		0, 0, 0, 0,  // compression
	    		0, 0, 0, 0,  // data size
	    		0, 0, 0, 0,  // h resolution
	    		0, 0, 0, 0,  // v resolution 
	    		0, 0, 0, 0,  // used colors
	    		0, 0, 0, 0   // important colors
	  	};
*/		
		unsigned int file_size;           // file size
	  
	  	image_t = (unsigned char *)malloc((size_t)width * height * 3);
	  	if (image_t == NULL) 
	  	{
	    	printf("malloc image_t error\n");
	    	return -1;
	  	}
		
		x_avg = 0 + (width-1);
		
		for(y = 0; y != height; ++y)
		{
			for(x = 0; x != width; ++x)
			{
				if ((x != 0) && (x != (width-1) ) && (y != 0) && (y != (height-1)))		//inside
					{
						if ((y%2) == 0)
							{
								if ((x%2) == 0)
									{
										R = *(pImageBuffer + x + y*width);
									 	G = (*(pImageBuffer + (x+1) + y*width)+*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + x + (y+1)*width)+*(pImageBuffer + x + (y-1)*width))/4;
									  	B = (*(pImageBuffer + (x+1) + (y+1)*width)+*(pImageBuffer + (x-1) + (y+1)*width)+*(pImageBuffer + (x+1) + (y-1)*width)+*(pImageBuffer + (x-1) + (y-1)*width))/4;
									}
								else
									{
									  R = (*(pImageBuffer + (x+1) + y*width)+*(pImageBuffer + (x-1) + y*width))/2;
									  G = *(pImageBuffer + x + y*width);
									  B = (*(pImageBuffer + x + (y+1)*width)+*(pImageBuffer + x + (y-1)*width))/2;
									}
							}
						else
							{
									if ((x%2) != 0)
									{
										R = (*(pImageBuffer + (x+1) + (y+1)*width)+*(pImageBuffer + (x-1) + (y+1)*width)+*(pImageBuffer + (x+1) + (y-1)*width)+*(pImageBuffer + (x-1) + (y-1)*width))/4;
									  	G = (*(pImageBuffer + (x+1) + y*width)+*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + x + (y+1)*width)+*(pImageBuffer + x + (y-1)*width))/4;
									  	B = *(pImageBuffer + x + y*width);
									}
								else
									{
									  	R = (*(pImageBuffer + x + (y+1)*width)+*(pImageBuffer + x + (y-1)*width))/2;
									  	G = *(pImageBuffer + x + y*width);
										B = (*(pImageBuffer + (x+1) + y*width)+*(pImageBuffer + (x-1) + y*width))/2;
									}
							}
					}
					else if ((x == 0) && (y ==0))		//top left
						{
							R = *(pImageBuffer + x + y*width);
							G = (*(pImageBuffer + (x+1) + y*width)+*(pImageBuffer + x + (y+1)*width))/2;
							B = *(pImageBuffer + (x+1) + (y+1)*width);
						}
					else if ((x == 0) && (y == (height-1)))		//bottom left
						{
							R = *(pImageBuffer + x + (y-1)*width);
							G = *(pImageBuffer + x + y*width);
							B = *(pImageBuffer + (x+1) + y*width);
						}
					else if ((x == (width-1)) && (y == 0))		//top right
						{
							R = *(pImageBuffer + (x-1) + y*width);
							G = *(pImageBuffer + x + y*width);
							B = *(pImageBuffer + x + (y+1)*width);
						}
					else if ((x == (width-1)) && ( y == (height-1)))	// bottom right
						{
							R = *(pImageBuffer + (x-1) + (y-1)*width);
							G = (*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + x + (y-1)*width))/2;
							B = *(pImageBuffer + x + y*width);
						}
					else		//boundary
						{
							if (y == 0)		//top
								{
									if ((x%2) == 0)
										{
											R = *(pImageBuffer + x + y*width);
											G = (*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + (x+1) + y*width)+*(pImageBuffer + x + (y+1)*width))/3;
											B = (*(pImageBuffer + (x-1) + (y+1)*width)+*(pImageBuffer + (x+1) + (y+1)*width))/2;
										}
									else
										{
											R = (*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + (x+1) + y*width))/2;
											G = *(pImageBuffer + x + y*width);
											B = *(pImageBuffer + x + (y+1)*width);
										}
								}
							else if (y == (height-1))		//bottom
								{
									if ((x%2) != 0)
										{
											R = (*(pImageBuffer + (x-1) + (y-1)*width)+*(pImageBuffer + (x+1) + (y-1)*width))/2;
											G = (*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + (x+1) + y*width)+*(pImageBuffer + x + (y-1)*width))/3;
											B = *(pImageBuffer + x + y*width);
										}
									else
										{
											R = *(pImageBuffer + x + (y-1)*width);
											G = *(pImageBuffer + x + y*width);
											B = (*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + (x+1) + y*width))/2;
										}
								}
								else if (x == 0)		//left
									{
									if ((y%2) == 0)
										{
											R = *(pImageBuffer + x + y*width);
											G = (*(pImageBuffer + (x+1) + y*width)+*(pImageBuffer + x + (y-1)*width)+*(pImageBuffer + x + (y+1)*width))/3;
											B = (*(pImageBuffer + (x+1) + (y+1)*width)+*(pImageBuffer + (x+1) + (y-1)*width))/2;
										}
									else
										{
											R = (*(pImageBuffer + x + (y+1)*width)+*(pImageBuffer + x + (y-1)*width))/2;
											G = *(pImageBuffer + x + y*width);
											B = *(pImageBuffer + (x+1) + y*width);
										}
								  }
								else if (x == (width-1))		//right
								{
									if ((y%2) != 0)
										{
											R = (*(pImageBuffer + (x-1) + (y-1)*width)+*(pImageBuffer + (x-1) + (y+1)*width))/2;
											G = (*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + x + (y+1)*width)+*(pImageBuffer + x + (y-1)*width))/3;
											B = *(pImageBuffer + x + y*width);
										}
									else
										{
											R = *(pImageBuffer + (x-1) + y*width);
											G = *(pImageBuffer + x + y*width);
											B = (*(pImageBuffer + x + (y-1)*width)+*(pImageBuffer + x + (y+1)*width))/2;
										}
								}
						}
				//y_t = x_avg - x;
	        
	      *(image_t + 3 * (width * y + x) + 2) = R;
	      *(image_t + 3 * (width * y + x) + 1) = G;
	      *(image_t + 3 * (width * y + x) + 0) = B;
					
			}
		}
	
		
	  fp_t = fopen("lab8_1.bmp", "wb");
	  if (fp_t == NULL) 
	  {
	    printf("fopen fname_t error\n");
	    return -1;
	  }
	        
	     // file size  
	  file_size = width * height * 3 + 54;
/*	  header[2] = (unsigned char)(file_size & 0x000000ff);
	  header[3] = (file_size >> 8)  & 0x000000ff;
	  header[4] = (file_size >> 16) & 0x000000ff;
	  header[5] = (file_size >> 24) & 0x000000ff;
	     
	     // width
	  header[18] = width & 0x000000ff;
	  header[19] = (width >> 8)  & 0x000000ff;
	  header[20] = (width >> 16) & 0x000000ff;
	  header[21] = (width >> 24) & 0x000000ff;
	     
	     // height
	  header[22] = height &0x000000ff;
	  header[23] = (height >> 8)  & 0x000000ff;
	  header[24] = (height >> 16) & 0x000000ff;
	  header[25] = (height >> 24) & 0x000000ff;
*/	   
	    // write header
//	  fwrite(header, sizeof(unsigned char), 54, fp_t);
//	  fwrite(&header, sizeof(header), 1, fp_t);
	  fwrite(&Header, sizeof Header, 1, fp_t); // write header 
	    // write image
	  fwrite(image_t, sizeof(unsigned char), (size_t)(long)width * height * 3, fp_t);
	     
	  fclose(fp_t);

	  system("./bin/cjpeg -quality 60 -outfile /lab8.jpg lab8.bmp"); 
	
		
  //}
	
	if(pImageBuffer)
		free(pImageBuffer);
//	ioctl(fd_cmos,CMOS_OFF,&CMOSInfo);
//	close(fd_cmos);
}
	
