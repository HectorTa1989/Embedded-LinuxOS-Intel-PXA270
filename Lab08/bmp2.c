// http://read.pudn.com/downloads178/sourcecode/windows/multimedia/826299/raw2bmp.c__.htm
#include <stdio.h>   
#include <string.h>   
   
#pragma pack( 1 )   
   
#define WORD unsigned short int   
#define DWORD unsigned int   
#define LONG unsigned long int   
   
#define BI_RGB 0x00000000   
   
typedef struct tagBITMAPFILEHEADER {    
  WORD    bfType;    
  DWORD   bfSize;    
  WORD    bfReserved1;    
  WORD    bfReserved2;    
  DWORD   bfOffBits;    
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;    
   
typedef struct tagBITMAPINFOHEADER{   
  DWORD  biSize;    
  LONG   biWidth;    
  LONG   biHeight;    
  WORD   biPlanes;    
  WORD   biBitCount;    
  DWORD  biCompression;    
  DWORD  biSizeImage;    
  LONG   biXPelsPerMeter;    
  LONG   biYPelsPerMeter;    
  DWORD  biClrUsed;    
  DWORD  biClrImportant;    
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;    
   
typedef struct rgbtag   
{   
    unsigned char b;   
    unsigned char g;   
    unsigned char r;   
} rgbtriple;   
#pragma pack( 4 )   
   
DWORD swap_16(DWORD dword)   
{   
    return ((dword>>8)&0xFF)+((dword&0xFF)<<8);   
}   
int main(int argc, char* argv[])   
{   
    int x;   
    int y;   
    FILE* rawdata;   
    FILE* bitmap;   
   
    // write the bitmap header   
    BITMAPFILEHEADER bmphead;   
    BITMAPINFOHEADER bmpinfohead;   
    rgbtriple pixels[640][480];   
    unsigned char input_file_name[1024];   
    unsigned char output_file_name[1024] ;   
    unsigned short dword;   
   
    bmphead.bfType = 19778;   
    bmphead.bfSize = sizeof(bmphead) + sizeof(bmpinfohead) + 640*480*3;   
    bmphead.bfReserved1 = bmphead.bfReserved2 = 0;   
    bmphead.bfOffBits = sizeof(bmphead) + sizeof(bmpinfohead);   
   
    // init structures   
    bmpinfohead.biSize = sizeof(bmpinfohead);   
    bmpinfohead.biWidth = 640;   
    bmpinfohead.biHeight = 480;   
    bmpinfohead.biBitCount = 24;   
    bmpinfohead.biPlanes = 1;   
    bmpinfohead.biCompression = BI_RGB;   
    bmpinfohead.biSizeImage = 0;   
    bmpinfohead.biClrImportant = 0;   
    bmpinfohead.biClrUsed = 0;   
   
    printf( "WORD %d\n", sizeof( WORD ) );   
    printf( "DWORD %d\n", sizeof( DWORD ) );   
    printf( "LONG %d\n", sizeof( LONG ) );   
    printf( "BITMAPFILEHEADER %d\n", sizeof( BITMAPFILEHEADER ) );   
    printf( "BITMAPINFOHEADER %d\n", sizeof( BITMAPINFOHEADER ) );   
    printf( "rgbtriple %d\n", sizeof(rgbtriple) );   
    printf( " unsigned char %d\n", sizeof( unsigned char ) );   
   
    // the program assumes the following:   
    // the image is 640x480 in 5.6.5 format   
    // must be converted to a 640x480 24-bit bitmap file   
    if(argc < 2) {   
        printf("Usage: raw2bmp <raw_file_name>\n");   
        return -1;   
    }   
//    strcpy( input_file_name, argv[1] );   
//    sprintf( output_file_name, "/tmp/picture.bmp" );   
       
       
//    rawdata = fopen(input_file_name, "rb");   
	rawdata = fopen("CF000915.IIQ", "rb");  
    if(rawdata == NULL) {   
        printf( "Could not open %s\n", input_file_name );   
        return -1;   
    }   
//    bitmap  = fopen(output_file_name, "wb");   
	bitmap  = fopen("2.bmp", "wb");   
    if(bitmap == NULL) {   
        printf( "Could not open %s\n", output_file_name );   
        return -1;   
    }   
   
    memset( pixels, 0,  sizeof(pixels[640][480]) );   
   
           
    for(x = 0; x < 640; x++)   
    {   
        for(y = 0; y < 480; y++)   
        {   
   
            fread((void*)&dword, 2, 1, rawdata);   
            //dword = swap_16(dword);   
/*          pixels[x][y].r = ((dword>>10) & 0x001F)<<3;  
            pixels[x][y].g = ((dword>>5)  & 0x001F)<<3;  
            pixels[x][y].b = ((dword)     & 0x001F)<<3;  
*/   
   
            pixels[x][y].r = ((dword>>11) & 0x001F)<<3;   
            pixels[x][y].g = ((dword>>5)  & 0x003F)<<2;   
            pixels[x][y].b = ((dword)     & 0x001F)<<3;   
   
            if( pixels[x][y].r < 12)   
                pixels[x][y].r = 12;   
            if( pixels[x][y].g < 12)   
                pixels[x][y].g = 12;   
            if( pixels[x][y].b < 12)   
                pixels[x][y].b = 12;   
            /*                       RGB 565  
            pixels[x][y].r = ((dword>>11) & 0x001F)<<3;  
            pixels[x][y].g = ((dword>>5)  & 0x003F)<<2;  
            pixels[x][y].b = ((dword)     & 0x001F)<<3;  
            */   
        }   
    }   
   
    fwrite(&bmphead, sizeof(bmphead), 1, bitmap);   
    fwrite(&bmpinfohead, sizeof(bmpinfohead), 1, bitmap);   
    fwrite(pixels, sizeof(pixels), 1, bitmap);   
   
    fclose(bitmap);   
    fclose(rawdata);   
   
    return 0;   
}   
