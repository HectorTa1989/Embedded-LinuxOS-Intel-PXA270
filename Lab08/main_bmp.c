#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/fcntl.h>
//#include"asm-arm/arch-pxa/lib/creator_pxa270_cmos.h"
/*
unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
unsigned char bmppad[3] = {0,0,0};
*/
unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
unsigned char bmppad[3] = {0,0,0};
int main()
{
//	Header* hd;
	int terminate = 1;
	int dwTotalReadSize,dwImageSize,nRead;
	int count, i, j;
	int fd_cmos;
//	cmos_info_t CMOSInfo;
	unsigned char *pImageBuffer = NULL;
	FILE *f;
	FILE *fp_t;
		unsigned int  width = 160,height = 120, depth = 24;
	//	unsigned int  width = 644,height = 480, depth = 24;
		unsigned int  x,y, yres, xres;
		unsigned char R, G, B;
		unsigned char *image_t = NULL;
		unsigned int  x_avg;            
		unsigned int  y_t;

//	dwImageSize = 160*120;
	dwImageSize = width*height;
//	pImageBuffer = malloc(dwImageSize);
// pImageBuffer to test code
	pImageBuffer = malloc(sizeof( unsigned char )*width*height*3);
/*	fd_cmos = open("/dev/cmos",O_RDWR);
	CMOSInfo.ImageSizeMode = CMOS_SIZE_MODE_160_120;
	CMOSInfo.HighRef = 120;
	ioctl(fd_cmos,CMOS_PARAMETER,&CMOSInfo);
	ioctl(fd_cmos,CMOS_ON,&CMOSInfo);
	
	while(terminate)
	{
		if(ioctl(fd_cmos,CMOS_GET_STATUS,&CMOSInfo)>=0)
		if (CMOSInfo.Status == CMOS_IMG_READY) 
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
	}*/

int filesize = 54 + 3*width * height;  //w is your image width, h is image height, both int
if( image_t )
    free( image_t );
image_t = (unsigned char *)malloc(width * height * 3);
	  	if (image_t == NULL) 
	  	{
	    	printf("malloc image_t error\n");
	    	return -1;
	  	}
memset(image_t,0,sizeof(image_t));

//unsigned int file_size;           // file size
	  
	  	
		
//////////// Open raw image file to test ////////////////
	f = fopen( "CF000915.IIQ", "rb" );
	fread( pImageBuffer, sizeof( unsigned char ), (size_t) (long) width*height*3, f );

//////////////////////////////				//////////////////
/*
for(int i=0; i<width; i++)
{
    for(int j=0; j<height; j++)
{
    x=i; y=(yres-1)-j;
    R = red[i][j]*255;
    G = green[i][j]*255;
    B = blue[i][j]*255;
    if (R > 255) R=255;
    if (G > 255) G=255;
    if (B > 255) B=255;

    img[(x+y*width)*3+2] = (unsigned char)(R);
    img[(x+y*width)*3+1] = (unsigned char)(G);
    img[(x+y*width)*3+0] = (unsigned char)(B);

    image_t[(x+y*width)*3+2] = (unsigned char)(R);
    image_t[(x+y*width)*3+1] = (unsigned char)(G);
    image_t[(x+y*width)*3+0] = (unsigned char)(B);

}
}
*/
x_avg = 0 + (width-1);
		
/*		for(y = 0; y != height; ++y)
		{
			for(x = 0; x != width; ++x)
			{*/
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
/*
	      *(image_t + 3 * (width * y + x) + 2) = R;
	      *(image_t + 3 * (width * y + x) + 1) = G;
	      *(image_t + 3 * (width * y + x) + 0) = B;	
*/
    image_t[(x+y*width)*3+2] = (unsigned char)(R);
    image_t[(x+y*width)*3+1] = (unsigned char)(G);
    image_t[(x+y*width)*3+0] = (unsigned char)(B);	
			}
		}

bmpfileheader[ 2] = (unsigned char)(filesize    );
bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
bmpfileheader[ 4] = (unsigned char)(filesize>>16);
bmpfileheader[ 5] = (unsigned char)(filesize>>24);

bmpinfoheader[ 4] = (unsigned char)(       width    );
bmpinfoheader[ 5] = (unsigned char)(       width>> 8);
bmpinfoheader[ 6] = (unsigned char)(       width>>16);
bmpinfoheader[ 7] = (unsigned char)(       width>>24);
bmpinfoheader[ 8] = (unsigned char)(       height    );
bmpinfoheader[ 9] = (unsigned char)(       height>> 8);
bmpinfoheader[10] = (unsigned char)(       height>>16);
bmpinfoheader[11] = (unsigned char)(       height>>24);

fp_t = fopen("img.bmp","wb");
fwrite(bmpfileheader,1,14,fp_t);
fwrite(bmpinfoheader,1,40,fp_t);
for(i=0; i<height; i++)
{
    fwrite(image_t+(width*(height-i-1)*3),3,width,fp_t);
    fwrite(bmppad,1,(4-(width*3)%4)%4,fp_t);
}
fclose(f);
fclose(fp_t);

	if(pImageBuffer)
		free(pImageBuffer);
/*	ioctl(fd_cmos,CMOS_OFF,&CMOSInfo);
	close(fd_cmos);*/
}
