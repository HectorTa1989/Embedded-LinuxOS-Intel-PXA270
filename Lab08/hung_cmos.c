#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/fcntl.h>
//#include"asm-arm/arch-pxa/lib/creator_pxa270_cmos.h"


int main()
{
	int terminate = 1;
	int dwTotalReadSize,dwImageSize,nRead;
	int count;
	int fd_cmos;
//	cmos_info_t CMOSInfo;
	unsigned char *pImageBuffer = NULL;
	
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
		//data process
		
		unsigned int  width = 160,height = 120, depth = 24;
		unsigned int  x,y;
		int i;
		unsigned char R, G, B;	
		unsigned char *image_t = NULL;
		unsigned int  x_avg;            
		unsigned int  y_t; 
	  	FILE          *fp_t = NULL;
		FILE 		*ff = NULL;
		ff = fopen("1.bmp","rb");
	unsigned char *Buffer = NULL;
	Buffer = malloc(sizeof( unsigned char )*width*height*3);
//	fseek(ff,54,SEEK_SET);
	fread( Buffer, sizeof( unsigned char ), (size_t) (long) width*height*3, ff );

		unsigned char header[54] = 
		{	//Buffer
/*	  		0x42,        // identity : B
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
	*/
			0x42,        // identity : B
			0x4d,        // identity : M
	    		/* 0x38*/0x36, 0xE1, 0, 0,  // file size
	   		0, 0,        // reserved1
	    		0, 0,        // reserved2
			0x36, 0, 0, 0, // RGB data offset
	    		0x28, 0, 0, 0, // struct BITMAPINFOHEADER size
	    		/*0xA0*/width, 0, 0, 0,  // bmp width
	    		/*0x78*/height, 0, 0, 0,  // bmp height
	    		1, 0,        // planes
	    		24, 0,       // bit per pixel
	    		0, 0, 0, 0,  // compression
	    		/*0x02*/0, 0xE1, 0, 0,  // data size
	    		/*0x5F, 0x09*/0, 0, 0, 0,  // h resolution
	    		/*0x5F, 0x09*/0,0, 0, 0,  // v resolution 
	    		0, 0, 0, 0,  // used colors
	    		0, 0, 0, 0   // important colors
	  	};
unsigned char bmppad[3] = {0,0,0};

	 // pImageBuffer to test code
		pImageBuffer = malloc(sizeof( unsigned char )*width*height*3);
		FILE 		*f = NULL;
//////////// Open raw image file to test ////////////////
//	f = fopen( "CF000915.IIQ", "rb" );
	f = fopen( "img.bin", "rb" );
//	fseek(f,sizeof(char)header[10],SEEK_SET);
	fseek(f,54,SEEK_SET);
	fread( pImageBuffer, sizeof( unsigned char ), (size_t) (long) width*height*3, f );

//////////////////////////////				//////////////////
		
		unsigned int file_size;           // file size
	  
	  	image_t = (unsigned char *)malloc((size_t)width * height * 3);
	  	if (image_t == NULL) 
	  	{
	    	printf("malloc image_t error\n");
	    	return -1;
	  	}

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
				//y_t = x_avg - x;
	        
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
	
		
	  fp_t = fopen("hung_cmos.bmp", "wb");
	  if (fp_t == NULL) 
	  {
	    printf("fopen fname_t error\n");
	    return -1;
	  }
	        
	     // file size  
	  file_size = width * height * 3 + 54;
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
	   
	    // write header
//	  fwrite(header, sizeof(unsigned char), 54, fp_t);
//	  fwrite(header, 1, 54, fp_t);
fwrite(&header, sizeof(header), 1, fp_t);
	    // write image
fseek(fp_t,54,SEEK_SET);
	  fwrite(image_t, sizeof(unsigned char), (size_t)(long)width * height * 3, fp_t);
//fwrite(&image_t, sizeof(image_t), 1, fp_t);
/*
for(i=0; i<height; i++)
{
    fwrite(image_t+(width*(height-i-1)*3),3,width,fp_t);
    fwrite(bmppad,1,(4-(width*3)%4)%4,fp_t);
}*/	     
	  fclose(fp_t);
	  fclose(f);
//	  system("./bin/cjpeg -quality 60 -outfile /lab8.jpg lab8.bmp"); 
//	system("cd ./longhung/EOS/lab8/cjpeg -quality 60 -outfile /lab8.jpg download.bmp");
	system("sudo apt-get install libjpeg-progs");
	system("cjpeg -quality 60 hung_cmos.bmp > hung_cmos.jpg");
//	cjpeg -quality 60 download.bmp > foo.jpg
//	cjpeg -progressive download.bmp >fa.jpg
//	cjpeg -optimize download.bmp >fa.jpg
		
  //}
	
	if(pImageBuffer)
		free(pImageBuffer);
//	ioctl(fd_cmos,CMOS_OFF,&CMOSInfo);
//	close(fd_cmos);
}
	
