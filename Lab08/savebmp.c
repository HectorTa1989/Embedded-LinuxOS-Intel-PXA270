#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/fcntl.h>
#include "qdbmp.h"
 
#define byte unsigned char
#define dword unsigned int 
#define word unsigned short int 
 
#pragma pack (1)
 
//4 Bytes per each pallete color
typedef struct {
    unsigned char rgbBlue,    
                 rgbGreen,    
                   rgbRed,    
              rgbReserved;    
}BITMAP_PALETTE;
 
// BMP File header
typedef struct bitmap_file_header {
    byte bfType[2];    
    dword bfSize;
    word bfReserved1;    
    word bfReserved2;
    dword bfOffBits;
} BITMAP_FILE_HEADER;
 
// BMP Image header
typedef struct bitmap_info_header {
    dword biSize;
    dword biWidth;
    dword biHeight;
    word biPlanes;                   
    word biBitCount;
    dword biCompression;       
    dword biSizeImage;
    dword biXPelsPerMeter;     
    dword biYPelsPerMeter;
    dword biClrUsed;               
    dword biClrImportant;
} BITMAP_INFO_HEADER;
 
typedef struct image
{
    BITMAP_FILE_HEADER file_info;
    BITMAP_INFO_HEADER bitmap_info;
    BITMAP_PALETTE *bitmap_palette;
    byte *data;
} IMAGE;
 
int save_image(IMAGE *image, char name[])
{
    FILE *fp;
    int i, rowsize;
    byte temp;
    unsigned int row, col, colour;
    int nx, ny;
 
    if(image == NULL)
        return EXIT_FAILURE;
    fp=fopen(name, "wb");
    if(fp==NULL)
    {
        printf("cannot open the file %s\n", name);
        return EXIT_FAILURE;
    }
    fwrite(&image->file_info, sizeof(BITMAP_FILE_HEADER),1,fp);
    fwrite(&image->bitmap_info, sizeof(BITMAP_INFO_HEADER),1,fp);
    int itotalPalettbytes = (int) image->file_info.bfOffBits - sizeof(BITMAP_FILE_HEADER) - (int)sizeof(BITMAP_INFO_HEADER);
    fwrite((void *)(&image->bitmap_palette),4, itotalPalettbytes / 4, fp);
    if (((image->bitmap_info.biWidth * 3) % 4) == 0) {
        nx = image->bitmap_info.biWidth * 3;
    } 
    else {
        nx = image->bitmap_info.biWidth * 3 + 4 -((image->bitmap_info.biWidth * 3) % 4);
    }
    ny = image->bitmap_info.biHeight;
    fwrite((void *)image->data, sizeof(byte),nx * ny ,  fp);
    fclose(fp);
    return EXIT_SUCCESS;
}
 
int main(int argc, char **argv)
{
    int iNx = 9, iNy;
    FILE *fp;
    int iTotalPaletteBytes;
    unsigned short int iType;     //holds the first two bytes of the file
    IMAGE *ImageInput = (IMAGE *) malloc(sizeof(IMAGE));

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
		
		unsigned int  width = 160,height = 120;
		unsigned int  x,y;
		unsigned char R, G, B;
		unsigned char *image_t = NULL;
		unsigned int  x_avg;            
		unsigned int  y_t; 
	  	FILE          *fp_t = NULL;
	  // pImageBuffer to test code
		pImageBuffer = malloc(sizeof( unsigned char )*width*height*3);
		FILE 		*f = NULL;
		unsigned int file_size;           // file size
	  
	  	image_t = (unsigned char *)malloc((size_t)width * height * 3);
	  	if (image_t == NULL) 
	  	{
	    	printf("malloc image_t error\n");
	    	return -1;
	  	}
//////////// Open raw image file to test ////////////////
	f = fopen( "CF000915.IIQ", "rb" );
//	f = fopen( "download.bmp", "rb" );
	fread( pImageBuffer, sizeof( unsigned char ), (size_t) (long) width*height*3, f );

//////////////////////////////				//////////////////


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
	
		
	  fp_t = fopen("hung_cmos.bmp", "wb");
	  if (fp_t == NULL) 
	  {
	    printf("fopen fname_t error\n");
	    return -1;
	  }
// write image
	  fwrite(image_t, sizeof(unsigned char), (size_t)(long)width * height * 3, fp_t);
//fwrite(&image_t, sizeof(image_t), 1, fp_t);
	     
	  fclose(fp_t);

	  system("./bin/cjpeg -quality 60 -outfile /lab8.jpg lab8.bmp"); 
	
		
  //}
	
	if(pImageBuffer)
		free(pImageBuffer);
//	ioctl(fd_cmos,CMOS_OFF,&CMOSInfo);
//	close(fd_cmos);
}
