// http://stackoverflow.com/questions/6912601/write-ximage-to-bmp-file-in-c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/fcntl.h>
#include "qdbmp.h"
#define BMP_PALETTE_SIZE	( 256 * 4 )
//#define BMP_PALETTE_SIZE	( 16777216 * 4 )

//#pragma pack (1) //Also are you sure that your structures are not getting padded (maybe you need 	__attribute__((packed(1)))?  (Fix Premature end-of-file error)

//#include"asm-arm/arch-pxa/lib/creator_pxa270_cmos.h"

//void saveXImageToBitmap(XImage *pImage);
#pragma pack (push)
#pragma pack (1)
typedef struct tagBITMAPFILEHEADER {
/*//WORD    bfType;
USHORT	bfType;
DWORD   bfSize;
WORD    bfReserved1;
WORD    bfReserved2;
DWORD   bfOffBits;*/
	USHORT		bfType;				/* Magic identifier: "BM" */
	UINT		bfSize;			/* Size of the BMP file in bytes */
	USHORT		bfReserved1;			/* Reserved */
	USHORT		bfReserved2;			/* Reserved */
	UINT		bfOffBits;			/* Offset of image data relative to the file's start */
	
} BITMAPFILEHEADER;
#pragma pack (pop)

#pragma pack (push)
#pragma pack (1)
typedef struct tagBITMAPINFOHEADER{
/*//DWORD  biSize;
UINT	biSize;
LONG   biWidth;
LONG   biHeight;
WORD   biPlanes;
WORD   biBitCount;
DWORD  biCompression;
DWORD  biSizeImage;
LONG   biXPelsPerMeter;
LONG   biYPelsPerMeter;
DWORD  biClrUsed;
DWORD  biClrImportant;*/
	UINT		biSize;			/* Size of the header in bytes */
	UINT		biWidth;				/* Bitmap's width */
	UINT		biHeight;				/* Bitmap's height */
	USHORT		biPlanes;				/* Number of color planes in the bitmap */
	USHORT		biBitCount;		/* Number of bits per pixel */
	UINT		biCompression;	/* Compression type */
	UINT		biSizeImage;		/* Size of uncompressed image's data */
	UINT		biXPelsPerMeter;	/* Horizontal resolution (pixels per meter) */
	UINT		biYPelsPerMeter;	/* Vertical resolution (pixels per meter) */
	UINT		biClrUsed;			/* Number of color indexes in the color table that are actually used by the bitmap */
	UINT		biClrImportant;		/* Number of color indexes that are required for displaying the bitmap */
} BITMAPINFOHEADER;
#pragma pack (pop)


int main()
{
	int terminate = 1;
	int dwTotalReadSize,dwImageSize,nRead;
	int count;
	int fd_cmos;
//	cmos_info_t CMOSInfo;
	unsigned char *pImageBuffer = NULL;
	
		unsigned int  width = 160,height = 120, depth = 24;
		unsigned int  x,y;
		unsigned char R, G, B;
		unsigned char *image_t = NULL;
		unsigned int  x_avg;            
		unsigned int  y_t; 
	  	FILE          *fp_t = NULL;
		int		bytes_per_pixel = depth >> 3;
		UINT	bytes_per_row;
	dwImageSize = 160*120;
//	pImageBuffer = malloc(dwImageSize);
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
// pImageBuffer to test code
		pImageBuffer = malloc(sizeof( unsigned char )*width*height*3);
		FILE 		*f = NULL;
		UCHAR		Data;
		UCHAR		Palette;
//////////// Open raw image file to test ////////////////
	f = fopen( "CF000915.IIQ", "rb" );
	fread( pImageBuffer, sizeof( unsigned char ), (size_t) (long) width*height*3, f );
/////////////////////////////////////////////////////////////


		//data process
		
BITMAPFILEHEADER bmpFileHeader;
BITMAPINFOHEADER bmpInfoHeader;
static int cnt = 0;
int dummy;
char filePath[255];
fp_t = fopen("lab8_2.bmp", "wb");
	  if (fp_t == NULL) 
	  {
	    printf("fopen fname_t error\n");
	    return -1;
	  }
memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));

bmpFileHeader.bfType = 0x4D42;
bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +  width*height*4;
bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
bmpFileHeader.bfReserved1 = 0;
bmpFileHeader.bfReserved2 = 0;

//bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
bmpInfoHeader.biSize = 0x28;
bmpInfoHeader.biWidth = width;
bmpInfoHeader.biHeight = height;
bmpInfoHeader.biPlanes = 1;
bmpInfoHeader.biBitCount = depth;
dummy = (width * 3) % 4;
if((4-dummy)==4)
    dummy=0;
else
    dummy=4-dummy;
bmpInfoHeader.biSizeImage = ((width*3)+dummy)*height;
bmpInfoHeader.biCompression = 0;
bmpInfoHeader.biXPelsPerMeter = 0;
bmpInfoHeader.biYPelsPerMeter = 0;
bmpInfoHeader.biClrUsed = 0;
bmpInfoHeader.biClrImportant = 0;
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

/*
bmpFileHeader.bfType = 0x4D42;
bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +  pImage->width*pImage->height*4;
bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
bmpFileHeader.bfReserved1 = 0;
bmpFileHeader.bfReserved2 = 0;

bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
bmpInfoHeader.biWidth = pImage->width;
bmpInfoHeader.biHeight = pImage->height;
bmpInfoHeader.biPlanes = 1;
bmpInfoHeader.biBitCount = 32;
dummy = (pImage->width * 3) % 4;
if((4-dummy)==4)
    dummy=0;
else
    dummy=4-dummy;
bmpInfoHeader.biSizeImage = ((pImage->width*3)+dummy)*pImage->height;
bmpInfoHeader.biCompression = 0;
bmpInfoHeader.biXPelsPerMeter = 0;
bmpInfoHeader.biYPelsPerMeter = 0;
bmpInfoHeader.biClrUsed = 0;
bmpInfoHeader.biClrImportant = 0;
*/


/*bmpFileHeader.bfType = 0x4D42;
bmpFileHeader.bfSize = 40;
bmpFileHeader.bfOffBits = 54 + ( depth == 24 ? BMP_PALETTE_SIZE : 0 );
bmpFileHeader.bfReserved1 = 0;
bmpFileHeader.bfReserved2 = 0;

bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
bmpInfoHeader.biWidth = width;
bmpInfoHeader.biHeight = height;
bmpInfoHeader.biPlanes = 1;
bmpInfoHeader.biBitCount = depth;

bytes_per_row = width * bytes_per_pixel;
bytes_per_row += ( bytes_per_row % 4 ? 4 - bytes_per_row % 4 : 0 );

bmpInfoHeader.biSizeImage = bytes_per_row * height;
bmpInfoHeader.biCompression = 0;
bmpInfoHeader.biXPelsPerMeter = 0;
bmpInfoHeader.biYPelsPerMeter = 0;
bmpInfoHeader.biClrUsed = 0;
bmpInfoHeader.biClrImportant = 0;
	*/
//sprintf(filePath, "bitmap%d.bmp", cnt++);
//fp = fopen(filePath,"wb");

if(fp_t == NULL)
    return;



		unsigned int file_size;           // file size
	  
	  	image_t = (unsigned char *)malloc((size_t)width * height * 3);
	  	if (image_t == NULL) 
	  	{
	    	printf("malloc image_t error\n");
	    	return -1;
	  	}
		

///////////////////////////////////////////////////
		x_avg = 0 + (width-1);
		
/*		for(y = 0; y != height; ++y)
		{
			for(x = 0; x != width; ++x)	*/
		for(y = 0; y < height; ++y)
		{
			for(x = 0; x < width; ++x)
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
							R = *(pImageBuffer + x + y*width);
							G = (*(pImageBuffer + (x+1) + y*width) + *(pImageBuffer + x + (y-1)*width))/2;
							B = *(pImageBuffer + (x+1) + (y-1)*width);
						}
					else if ((x == (width-1)) && (y == 0))		//top right
						{
							R = *(pImageBuffer + x + y*width);
							G = (*(pImageBuffer + x + (y+1)*width) + *(pImageBuffer + (x-1) + y*width))/2;
							B = *(pImageBuffer + (x-1) + (y+1)*width);
						}
					else if ((x == (width-1)) && ( y == (height-1)))	// bottom right
						{
							R = *(pImageBuffer + x + y*width);
							G = (*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + x + (y-1)*width))/2;
							B = *(pImageBuffer + (x-1) + (y-1)*width);
						}
					else		//boundary
						{
							if (y == 0)		//top
								{
									if (((x%2) == 0) && (x!=0) && (x!=width-1))
										{
											R = *(pImageBuffer + x + y*width);
											G = (*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + (x+1) + y*width)+*(pImageBuffer + x + (y+1)*width))/3;
											B = (*(pImageBuffer + (x-1) + (y+1)*width)+*(pImageBuffer + (x+1) + (y+1)*width))/2;
										}
									else if (((x%2) != 0) && (x!=0) && (x!=width-1))
										{
											R = (*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + (x+1) + y*width))/2;
											G = *(pImageBuffer + x + y*width);
											B = *(pImageBuffer + x + (y+1)*width);
										}
								}
							else if (y == (height-1))		//bottom
								{
									if (((x%2) != 0) && (x!=0) && (x!=(width-1)))
										{
											R = (*(pImageBuffer + (x-1) + y*width)+*(pImageBuffer + (x+1) + y*width))/2;
											G = *(pImageBuffer + x + y*width);
											B = *(pImageBuffer + x + (y-1)*width);
										}
									else if (((x%2) == 0) && (x!=0) && (x!=(width-1)))
										{
											R = *(pImageBuffer + x + y*width);
											G = (*(pImageBuffer + x + (y-1)*width) + *(pImageBuffer + (x+1) + y*width) + *(pImageBuffer + (x-1) + y*width))/3;
											B = (*(pImageBuffer + (x-1) + (y-1)*width)+*(pImageBuffer + (x+1) + (y-1)*width))/2;
										}
								}
								else if (x == 0)		//left
									{
									if (((y%2) == 0) && (y!=0) && (y!=(height-1)))
										{
											R = *(pImageBuffer + x + y*width);
											G = (*(pImageBuffer + (x+1) + y*width)+*(pImageBuffer + x + (y-1)*width)+*(pImageBuffer + x + (y+1)*width))/3;
											B = (*(pImageBuffer + (x+1) + (y+1)*width)+*(pImageBuffer + (x+1) + (y-1)*width))/2;
										}
									else if (((y%2) != 0) && (y!=0) && (y!=(height-1)))
										{
											R = (*(pImageBuffer + x + (y+1)*width)+*(pImageBuffer + x + (y-1)*width))/2;
											G = *(pImageBuffer + x + y*width);
											B = *(pImageBuffer + (x+1) + y*width);
										}
								  }
								else if (x == (width-1))		//right
								{
									if (((y%2) != 0) && (y!=0) && (y!=(height-1)))
										{
											R = (*(pImageBuffer + x + (y-1)*width)+*(pImageBuffer + x + (y+1)*width))/2;
											G = *(pImageBuffer + x + y*width);
											B = *(pImageBuffer + (x-1) + y*width);
										}
									else if (((y%2) == 0) && (y!=0) && (y!=(height-1)))
										{
											R = *(pImageBuffer + x + y*width);
											G = (*(pImageBuffer + (x-1) + y*width) + *(pImageBuffer + x + (y+1)*width) + *(pImageBuffer + x + (y-1)*width))/3;
											B = (*(pImageBuffer + (x-1) + (y-1)*width)+*(pImageBuffer + (x-1) + (y+1)*width))/2;
										}
								}
						}
				//y_t = x_avg - x;
	        
	      *(image_t + 3 * (width * y + x) + 2) = R;
	      *(image_t + 3 * (width * y + x) + 1) = G;
	      *(image_t + 3 * (width * y + x) + 0) = B;
					
			}
		} 
	        
	     // file size  
/*	  file_size = width * height * 3 + 54;
	  header[2] = (unsigned char)(file_size & 0x000000ff);
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
fwrite(&bmpFileHeader, sizeof(bmpFileHeader), 1, fp_t);
fwrite(&bmpInfoHeader, sizeof(bmpInfoHeader), 1, fp_t);
//fwrite(pImage->data, 4*pImage->width*pImage->height, 1, fp_t);
//fclose(fp);

	    // write image
//	  fwrite(image_t, sizeof(unsigned char), (size_t)(long)width * height * 3, fp_t);
	fwrite(&image_t, sizeof(image_t), 1, fp_t);
	     
	  fclose(fp_t);

	  system("./bin/cjpeg -quality 60 -outfile /lab8.jpg lab8.bmp"); 
	
		
  //}
	
	if(pImageBuffer)
		free(pImageBuffer);
//	ioctl(fd_cmos,CMOS_OFF,&CMOSInfo);
//	close(fd_cmos);
}
	
