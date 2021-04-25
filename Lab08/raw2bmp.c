// http://filedb.experts-exchange.com/incoming/2010/03_w10/260548/raw2bmp.c
//Raw data to Bitmap file conversation utility
//PRIMAX com.

#include <stdio.h>
#include <string.h>
#include <memory.h>

typedef unsigned char                   UInt8;
typedef signed char                     SInt8;
typedef unsigned short                  UInt16;
typedef signed short                    SInt16;

#if __LP64__
typedef unsigned int                    UInt32;
typedef signed int                      SInt32;
#else
typedef unsigned long                   UInt32;
typedef signed long                     SInt32;
#endif

typedef 	UInt32		DWORD;
typedef 	UInt16		WORD;
typedef 	unsigned char	BYTE;

#ifndef bool
typedef		unsigned short bool;
#define false 0
#define true 0
#endif

typedef	UInt32		LONG;

typedef struct tagBITMAPFILEHEADER
{
    UInt16    bfType;
    UInt32   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD   biSize;
    LONG    biWidth;
    LONG    biHeight;
    WORD    biPlanes;
    WORD    biBitCount;
    DWORD   biCompression;
    DWORD   biSizeImage;
    LONG    biXPelsPerMeter;
    LONG    biYPelsPerMeter;
    DWORD   biClrUsed;
    DWORD   biClrImportant;
} BITMAPINFOHEADER, * PBITMAPINFOHEADER; 

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
}RGBQUAD;

bool gbIsLittleEndian = false;

/*
 *  Usage:
 *      RawToBmp RawFileName Width Height BitNumber Convert Offset
 *      Convert==0. Donot swap RGB sequence
 *      Offset      Skip how many bytes from raw data file header
 */
int main (int argc, const char * argv[]) {
    // insert code here...
	FILE *fpSrc,*fpTgr;
	int Width,Height,bConvert,bOffset;
	int BitNumber;
	int Len;
	int TargetLength;
	char TempStr[512];
	unsigned char * Buffer;
	int i;
	BITMAPINFOHEADER BitmapInfoHeader;
	BITMAPFILEHEADER BitmapFileHeader;
	RGBQUAD rgbQuad[256];
	
	IsLittleEndian();
	
    printf("Raw to Bitmap convert utility!\n");
	if(argc<4)
	 {
		printf(" Invalid parameters!\n");
		printf(" Usage:  RawToBmp RawFileName Width Height BitNumber [Convert  Offset] \n");
		printf(" Note:   BitNumber must be 1,8 or 24\n");
		printf("         Convert==0. Donot swap RGB sequence\n");
		printf("         Offset      Skip how many bytes from raw data file header\n\n");
		return -1;
	 }
	 
	 //Get advanced options
	 //Convert
	 if(argc>=5)
		bConvert=atoi(argv[4]);
	 else
	    bConvert=0;

	 //Offset
	 if(argc>=6)
		bOffset=atoi(argv[5]);
	 else
	    bOffset = 0;
		
	//Get Width and Height
	Width = atoi(argv[2]);
	Height = atoi(argv[3]);
	if(Width<=0 || Height <= 0)
	  {
	    printf("Invalid Width or Height!\n");
		return -2;
	  }
	//Get Bitnumber per pixel
	BitNumber = atoi(argv[4]);
	if((BitNumber!=1) && (BitNumber!=8) && (BitNumber!=24))
	  {
	    printf("Invalid BitNumber! Must be 1,8 or 24!\n");
		return -3;
	  }
	//only True color mode can do RGB swapping
	if(BitNumber!=24)
	  bConvert = 0;
	  
	//Open the Source file
//	fpSrc = fopen(argv[1],"rb");
	fpSrc = fopen("CF000915.IIQ","rb");
	if(fpSrc==0)
	  {
	    printf("Cannot open the raw data file!\n");
		return -4;
	  }
	  
	//Get raw file name
	for(i=strlen(argv[1]);i>0;i--)
	  if(argv[1][i] == '.')
			break;
	strcpy(TempStr,argv[1]);	
	if(i!=0)
		TempStr[i]=0;
	else
		TempStr[strlen(argv[1])]=0;
		
	strcat(TempStr,".bmp\0");
	
	//Open the target file
//	fpTgr = fopen(TempStr,"wb+");
	fpTgr = fopen("b.bmp","wb+");
	if(fpTgr==0)
	{
		fclose(fpSrc);
		printf("Cannot create the bitmap file!\n");
		return -5;
	}
	
	//Write bitmap header
	SetBitmapInfoHeader(&BitmapFileHeader,&BitmapInfoHeader,Width,Height,BitNumber);
	Len=2;
    fwrite((unsigned char *)&BitmapFileHeader.bfType,Len,1,fpTgr);
    Len=4;
	fwrite((unsigned char *)&BitmapFileHeader.bfSize,Len,1,fpTgr);
    Len=2;
	fwrite((unsigned char *)&BitmapFileHeader.bfReserved1,Len,1,fpTgr);
    Len=2;
	fwrite((unsigned char *)&BitmapFileHeader.bfReserved2,Len,1,fpTgr);
    Len=4;
	fwrite((unsigned char *)&BitmapFileHeader.bfOffBits,Len,1,fpTgr);
    
    fwrite((unsigned char *)&BitmapInfoHeader,40,1,fpTgr);
	//if is gray bitmap,then write platte
	if(BitNumber == 8)
	{
	   for(i=0;i<256;i++)
	     rgbQuad[i].rgbBlue = rgbQuad[i].rgbGreen = rgbQuad[i].rgbRed = i;
	   fwrite((unsigned char *)rgbQuad,4,256,fpTgr);
	}
	else if (BitNumber == 1)
	{
		rgbQuad[0].rgbBlue = rgbQuad[0].rgbGreen = rgbQuad[0].rgbRed = 0;
		rgbQuad[1].rgbBlue = rgbQuad[1].rgbGreen = rgbQuad[1].rgbRed = 255;
	    fwrite((unsigned char *)rgbQuad,4,2,fpTgr);
	}
	
	//Calcuate the line size
	Width = (Width*BitNumber+7)/8; //ByteNumber per line
	TargetLength = ((Width+3)/4)*4;
	//Allocate the memory
	Buffer = (unsigned char *)malloc(TargetLength);
	printf("Processing now......\n");
	
	if(bOffset>0)
	  {
	    while(bOffset>0)
		  {
			 if(Width>bOffset)
			   {
			     fread(Buffer,1,bOffset,fpSrc);
				 bOffset=0;
			   }
			 else
			   {
				fread(Buffer,1,Width,fpSrc);
				bOffset -= Width;
			   }
		  }
	  }
	//Copy file now
	while(!feof(fpSrc))
	{
		fread(Buffer,1,Width,fpSrc);
		if(bConvert)
		 for(i=0;i<Width/3;i++)
		   {
		      BYTE temp = Buffer[i*3+2];
			  Buffer[i*3+2] = Buffer[i*3];
			  Buffer[i*3] = temp; 
		   }
		fwrite(Buffer,1,TargetLength,fpTgr);
	}
	
	printf("Sucess to convert the data!\n Enjoy it!\n");
	
	//Close files
	fclose(fpSrc);
	fclose(fpTgr);
	free(Buffer);
	
    return 0;
}

void  SetBitmapInfoHeader(BITMAPFILEHEADER * g_BitmapFileHeader,BITMAPINFOHEADER * g_BitmapInfoHeader,int Width,int Height,int nBitNumber)
{
    //BITMAPINFOHEADER g_BitmapInfoHeader;
    //BITMAPFILEHEADER g_BitmapFileHeader;
    DWORD m_dwDibSize;
    DWORD m_Width;
    DWORD m_Height;
    m_Width  = Width;//m_lScanRight-m_lScanLeft+1;
    m_Height = Height;//m_lScanBottom-m_lScanTop+1;
    m_dwDibSize = m_Width*m_Height*nBitNumber/8;// RGB,24Bits
	
    //BITMAPFILEHEADER
    g_BitmapFileHeader->bfType='MB';//DIFFERENCE
    g_BitmapFileHeader->bfSize=14+sizeof(BITMAPINFOHEADER)+m_dwDibSize;
    g_BitmapFileHeader->bfOffBits = 14+sizeof(BITMAPINFOHEADER);//NO PLATTE HERE!
	if(nBitNumber==8)
		g_BitmapFileHeader->bfOffBits+=256*4;
	else if(nBitNumber == 1)
		g_BitmapFileHeader->bfOffBits+=2*4;
    //Swap bits
	ShortSwapA(&g_BitmapFileHeader->bfType);
    LongSwapA(&g_BitmapFileHeader->bfSize);
    LongSwapA(&g_BitmapFileHeader->bfOffBits);
    
    g_BitmapFileHeader->bfReserved1 = 0;
    g_BitmapFileHeader->bfReserved2 = 0;
    
    //BITMAPINFOHEADER
    g_BitmapInfoHeader->biSize = 40;
    g_BitmapInfoHeader->biPlanes = 1;
    g_BitmapInfoHeader->biWidth = m_Width;
    g_BitmapInfoHeader->biHeight = -1*m_Height;
    g_BitmapInfoHeader->biBitCount = nBitNumber;
    g_BitmapInfoHeader->biCompression = 0;
    g_BitmapInfoHeader->biSizeImage = m_dwDibSize;
    g_BitmapInfoHeader->biXPelsPerMeter = 0;
    g_BitmapInfoHeader->biYPelsPerMeter = 0;
    g_BitmapInfoHeader->biClrUsed = 0;
    g_BitmapInfoHeader->biClrImportant = 0;
    //SWAP
    LongSwapA(&g_BitmapInfoHeader->biSize);
    LongSwapA(&g_BitmapInfoHeader->biWidth);
    LongSwapA(&g_BitmapInfoHeader->biHeight);
    ShortSwapA(&g_BitmapInfoHeader->biPlanes);
    LongSwapA(&g_BitmapInfoHeader->biSizeImage);
    ShortSwapA(&g_BitmapInfoHeader->biBitCount);
}


void LongSwapA(unsigned long *X) 
{
    if(!gbIsLittleEndian)
	*X = (((*X & 0x000000FF) << 24) 
		+ ((*X & 0x0000FF00) << 8)
		+ ((*X & 0x00FF0000) >> 8)
		+ ((*X & 0xFF000000) >> 24) );
}

void ShortSwapA(unsigned short *X) 
{
	if(!gbIsLittleEndian)
	*X = (((*X & 0x00FF) << 8)
		+ ((*X & 0xFF00) >> 8));
}

//check the byte-order of the current OS
void IsLittleEndian()
{
	unsigned char x[2];
	*(unsigned short *)x=0xff00;
	//Check byte-endian
	gbIsLittleEndian = (x[0]==0);
}

