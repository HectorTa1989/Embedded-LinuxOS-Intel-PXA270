#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/fcntl.h>
#include "qdbmp.h"
//#include"asm-arm/arch-pxa/lib/creator_pxa270_cmos.h"

#pragma pack (1)	//Also are you sure that your structures are not getting padded (maybe you need 	__attribute__((packed(1)))?  (Fix Premature end-of-file error)

#define BMP_PALETTE_SIZE	( 256 * 4 )
//#define BMP_PALETTE_SIZE	( 16777216 * 4 )
//unsigned int  width = 644,height = 480, depth = 24;
/* Bitmap header */
struct BMP_Header
{
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
//typedef struct BMP_Header Header;
	 	
int main()
{
//	Header* hd;
	int terminate = 1;
	int dwTotalReadSize,dwImageSize,nRead;
	int count;
	int fd_cmos;
//	cmos_info_t CMOSInfo;
	unsigned char *pImageBuffer = NULL;
	
		unsigned int  width = 160,height = 120, depth = 24;
	//	unsigned int  width = 644,height = 480, depth = 24;
		unsigned int  x,y;
		unsigned char R, G, B;
		unsigned char *image_t = NULL;
		unsigned int  x_avg;            
		unsigned int  y_t;

//	dwImageSize = 160*120;
	dwImageSize = width*height;
//	pImageBuffer = malloc(dwImageSize);
	
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

	
		//data process
		
 
// pImageBuffer to test code
		pImageBuffer = malloc(sizeof( unsigned char )*width*height*3);
	  	FILE          *fp_t = NULL;
		FILE 		*f = NULL;
		UCHAR		Data;
		UCHAR		Palette;
///Data = (UCHAR) malloc( Header.ImageDataSize );
//Palette = (UCHAR) malloc( BMP_PALETTE_SIZE * sizeof( UCHAR ) );	  
	Header.Magic				= 0x4D42;
	Header.Reserved1			= 0;
	Header.Reserved2			= 0;
	Header.HeaderSize			= 40; // Hex: 0x28
	Header.Planes				= 1;
	Header.CompressionType		= 0;
	Header.HPixelsPerMeter		= 0;
	Header.VPixelsPerMeter		= 0;
	Header.ColorsUsed		= 0;
	Header.ColorsRequired		= 0;
	Header.Width			= width;
	Header.Height			= height;
	Header.BitsPerPixel		= depth;
	Header.ImageDataSize		= 0xE1;        //width* height* depth E.g: a 4×2 pixel bitmap, with 32 bits/pixel (4 bytes/pixel) encoding would have Size of the raw bitmap data= 4*2*4= 32 bytes 
	Header.FileSize			= Header.ImageDataSize + 54 + ( depth == 24 ? BMP_PALETTE_SIZE : 0 );
	Header.DataOffset			= 54 + ( depth == 24 ? BMP_PALETTE_SIZE : 0 );
/*		unsigned char header[54] = 
		{
	  		0x42,        // identity : B
			0x4d,        // identity : M
	    		0xE26B6,  // file size
	   		0x00,        // reserved1
	    		0x00,        // reserved2
			0x36 ,	    // RGB data offset
	    		40,0,0,0,		// struct BITMAPINFOHEADER size
//	    		0, 0, 0, 0,  // bmp width
//	    		0, 0, 0, 0,  // bmp height
			0x644,	     // bmp width
			0x480 , 	     // bmp height
	    		0x01   ,      // planes
//	    		24, 0,       // bit per pixel
			0x18,       // bit per pixel
	    		0x00,  // compression
	    		0x4B780,  // data size
	    		0x00,  // h resolution
	    		0x00,  // v resolution 
	    		0x00,  // used colors
	    		0x00  // important colors    

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
		
//////////// Open raw image file to test ////////////////
	f = fopen( "CF000915.IIQ", "rb" );
	fread( pImageBuffer, sizeof( unsigned char ), (size_t) (long) width*height*3, f );

//////////////////////////////				//////////////////
		x_avg = 0 + (width-1);
		for(x = 0; x < width; ++x)
		{
			for(y = 0; y < height; ++y)
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
	
		
	  fp_t = fopen("lab8.bmp", "wb");
	  if (fp_t == NULL) 
	  {
	    printf("fopen fname_t error\n");
	    return -1;
	  }
	        
/*	     // file size  
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
*/	   
	    // write header
/*	if ( Palette )
	{
		if ( fwrite( Palette, sizeof( UCHAR ), BMP_PALETTE_SIZE, fp_t ) != BMP_PALETTE_SIZE )
		{
//			BMP_LAST_ERROR_CODE = BMP_IO_ERROR;
			fclose( fp_t );
			return;
		}
	}


	// Write data 
	if ( fwrite( Data, sizeof( UCHAR ), Header.ImageDataSize, fp_t ) != Header.ImageDataSize )
	{
//		BMP_LAST_ERROR_CODE = BMP_IO_ERROR;
		fclose( fp_t );
		return;
	}	*/
//	  fwrite(header, sizeof(unsigned char), 54, fp_t);
//	  fwrite(&Header, sizeof(Header), 1, fp_t);
	  fwrite(&Header, sizeof(char),sizeof(Header), fp_t);
	    // write image
	  fwrite(image_t, sizeof(unsigned char), (size_t)(long)width * height * 3, fp_t);
	     
	  fclose(fp_t);
	  fclose(f);

	  system("./bin/cjpeg -quality 60 -outfile /lab8.jpg lab8.bmp"); 
	
		
  //}
	
	if(pImageBuffer)
		free(pImageBuffer);
/*	ioctl(fd_cmos,CMOS_OFF,&CMOSInfo);
	close(fd_cmos);*/
}
	
