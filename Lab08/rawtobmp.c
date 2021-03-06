// https://www.gitorious.org/omap4-v4l2-camera/yavta/source/5417d27b99b2a147e3a062a24f36fd7a71f49b40:raw2bmp.c#L6
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>
 
#define TRUE -1
#define FALSE 0
 
typedef struct tagBITMAPFILEHEADER 
{ 
    uint16_t    bfType; //Specifies the file type, must be BM
    uint32_t   bfSize; //Specifies the size, in bytes, of the bitmap file
    uint16_t    bfReserved1; //Reserved; must be zero
    uint16_t    bfReserved2; //Reserved; must be zero
    uint32_t   bfOffBits; /*Specifies the offset, in bytes, 
                from the beginning of the BITMAPFILEHEADER structure 
                to the bitmap bits= FileHeader+InfoHeader+RGBQUAD(0 for 24bit BMP)=64*/
}   __attribute__ ((packed)) BITMAPFILEHEADER, *PBITMAPFILEHEADER;
 
 
typedef struct tagBITMAPINFOHEADER
{
    uint32_t  biSize; 
    uint32_t   biWidth; 
    uint32_t   biHeight; 
    uint16_t   biPlanes; 
    uint16_t   biBitCount; 
    uint32_t  biCompression; 
    uint32_t  biSizeImage; 
    uint32_t   biXPelsPerMeter; 
    uint32_t   biYPelsPerMeter; 
    uint32_t  biClrUsed; 
    uint32_t  biClrImportant; 
}  __attribute__ ((packed)) BITMAPINFOHEADER, *PBITMAPINFOHEADER;
 
static void convert_border_bayer_line_to_bgr24( uint8_t* bayer, uint8_t* adjacent_bayer,
    uint8_t *bgr, int width, int start_with_green, int blue_line)
{
    int t0, t1;
 
    if (start_with_green) 
    {
    /* First pixel */
        if (blue_line) 
        {
            *bgr++ = bayer[1];
            *bgr++ = bayer[0];
            *bgr++ = adjacent_bayer[0];
        } 
        else 
        {
            *bgr++ = adjacent_bayer[0];
            *bgr++ = bayer[0];
            *bgr++ = bayer[1];
        }
        /* Second pixel */
        t0 = (bayer[0] + bayer[2] + adjacent_bayer[1] + 1) / 3;
        t1 = (adjacent_bayer[0] + adjacent_bayer[2] + 1) >> 1;
        if (blue_line) 
        {
            *bgr++ = bayer[1];
            *bgr++ = t0;
            *bgr++ = t1;
        } 
        else 
        {
            *bgr++ = t1;
            *bgr++ = t0;
            *bgr++ = bayer[1];
        }
        bayer++;
        adjacent_bayer++;
        width -= 2;
    } 
    else 
    {
        /* First pixel */
        t0 = (bayer[1] + adjacent_bayer[0] + 1) >> 1;
        if (blue_line) 
        {
            *bgr++ = bayer[0];
            *bgr++ = t0;
            *bgr++ = adjacent_bayer[1];
        } 
        else 
        {
            *bgr++ = adjacent_bayer[1];
            *bgr++ = t0;
            *bgr++ = bayer[0];
        }
        width--;
    }
 
    if (blue_line) 
    {
        for ( ; width > 2; width -= 2) 
        {
            t0 = (bayer[0] + bayer[2] + 1) >> 1;
            *bgr++ = t0;
            *bgr++ = bayer[1];
            *bgr++ = adjacent_bayer[1];
            bayer++;
            adjacent_bayer++;
 
            t0 = (bayer[0] + bayer[2] + adjacent_bayer[1] + 1) / 3;
            t1 = (adjacent_bayer[0] + adjacent_bayer[2] + 1) >> 1;
            *bgr++ = bayer[1];
            *bgr++ = t0;
            *bgr++ = t1;
            bayer++;
            adjacent_bayer++;
        }
    } 
    else 
    {
        for ( ; width > 2; width -= 2) 
        {
            t0 = (bayer[0] + bayer[2] + 1) >> 1;
            *bgr++ = adjacent_bayer[1];
            *bgr++ = bayer[1];
            *bgr++ = t0;
            bayer++;
            adjacent_bayer++;
 
            t0 = (bayer[0] + bayer[2] + adjacent_bayer[1] + 1) / 3;
            t1 = (adjacent_bayer[0] + adjacent_bayer[2] + 1) >> 1;
            *bgr++ = t1;
            *bgr++ = t0;
            *bgr++ = bayer[1];
            bayer++;
            adjacent_bayer++;
        }
    }
 
    if (width == 2) 
    {
        /* Second to last pixel */
        t0 = (bayer[0] + bayer[2] + 1) >> 1;
        if (blue_line) 
        {
            *bgr++ = t0;
            *bgr++ = bayer[1];
            *bgr++ = adjacent_bayer[1];
        } 
        else 
        {
            *bgr++ = adjacent_bayer[1];
            *bgr++ = bayer[1];
            *bgr++ = t0;
        }
        /* Last pixel */
        t0 = (bayer[1] + adjacent_bayer[2] + 1) >> 1;
        if (blue_line) 
        {
            *bgr++ = bayer[2];
            *bgr++ = t0;
            *bgr++ = adjacent_bayer[1];
        }
        else 
        {
            *bgr++ = adjacent_bayer[1];
            *bgr++ = t0;
            *bgr++ = bayer[2];
        }
    } 
    else 
    {
        /* Last pixel */
        if (blue_line) 
        {
            *bgr++ = bayer[0];
            *bgr++ = bayer[1];
            *bgr++ = adjacent_bayer[1];
        } 
        else 
        {
            *bgr++ = adjacent_bayer[1];
            *bgr++ = bayer[1];
            *bgr++ = bayer[0];
        }
    }
}
 
static void bayer_to_rgbbgr24(uint8_t *bayer,
    uint8_t *bgr, int width, int height,
    int start_with_green, int blue_line)
{
    /* render the first line */
    convert_border_bayer_line_to_bgr24(bayer, bayer + width, bgr, width,
        start_with_green, blue_line);
    bgr += width * 3;
 
    /* reduce height by 2 because of the special case top/bottom line */
    for (height -= 2; height; height--) 
    {
        int t0, t1;
        /* (width - 2) because of the border */
        uint8_t *bayerEnd = bayer + (width - 2);
 
        if (start_with_green) 
        {
            /* OpenCV has a bug in the next line, which was
            t0 = (bayer[0] + bayer[width * 2] + 1) >> 1; */
            t0 = (bayer[1] + bayer[width * 2 + 1] + 1) >> 1;
            /* Write first pixel */
            t1 = (bayer[0] + bayer[width * 2] + bayer[width + 1] + 1) / 3;
            if (blue_line) 
            {
                *bgr++ = t0;
                *bgr++ = t1;
                *bgr++ = bayer[width];
            } 
            else 
            {
                *bgr++ = bayer[width];
                *bgr++ = t1;
                *bgr++ = t0;
            }
 
            /* Write second pixel */
            t1 = (bayer[width] + bayer[width + 2] + 1) >> 1;
            if (blue_line) 
            {
                *bgr++ = t0;
                *bgr++ = bayer[width + 1];
                *bgr++ = t1;
            } 
            else 
            {
                *bgr++ = t1;
                *bgr++ = bayer[width + 1];
                *bgr++ = t0;
            }
            bayer++;
        } 
        else 
        {
            /* Write first pixel */
            t0 = (bayer[0] + bayer[width * 2] + 1) >> 1;
            if (blue_line) 
            {
                *bgr++ = t0;
                *bgr++ = bayer[width];
                *bgr++ = bayer[width + 1];
            } 
            else 
            {
                *bgr++ = bayer[width + 1];
                *bgr++ = bayer[width];
                *bgr++ = t0;
            }
        }
 
        if (blue_line) 
        {
            for (; bayer <= bayerEnd - 2; bayer += 2) 
            {
                t0 = (bayer[0] + bayer[2] + bayer[width * 2] +
                    bayer[width * 2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[width] +
                    bayer[width + 2] + bayer[width * 2 + 1] +
                    2) >> 2;
                *bgr++ = t0;
                *bgr++ = t1;
                *bgr++ = bayer[width + 1];
 
                t0 = (bayer[2] + bayer[width * 2 + 2] + 1) >> 1;
                t1 = (bayer[width + 1] + bayer[width + 3] +
                    1) >> 1;
                *bgr++ = t0;
                *bgr++ = bayer[width + 2];
                *bgr++ = t1;
            }
        } 
        else 
        {
            for (; bayer <= bayerEnd - 2; bayer += 2) 
            {
                t0 = (bayer[0] + bayer[2] + bayer[width * 2] +
                    bayer[width * 2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[width] +
                    bayer[width + 2] + bayer[width * 2 + 1] +
                    2) >> 2;
                *bgr++ = bayer[width + 1];
                *bgr++ = t1;
                *bgr++ = t0;
 
                t0 = (bayer[2] + bayer[width * 2 + 2] + 1) >> 1;
                t1 = (bayer[width + 1] + bayer[width + 3] +
                    1) >> 1;
                *bgr++ = t1;
                *bgr++ = bayer[width + 2];
                *bgr++ = t0;
            }
        }
 
        if (bayer < bayerEnd) 
        {
            /* write second to last pixel */
            t0 = (bayer[0] + bayer[2] + bayer[width * 2] +
                bayer[width * 2 + 2] + 2) >> 2;
            t1 = (bayer[1] + bayer[width] +
                bayer[width + 2] + bayer[width * 2 + 1] +
                2) >> 2;
            if (blue_line) 
            {
                *bgr++ = t0;
                *bgr++ = t1;
                *bgr++ = bayer[width + 1];
            } 
            else 
            {
                *bgr++ = bayer[width + 1];
                *bgr++ = t1;
                *bgr++ = t0;
            }
            /* write last pixel */
            t0 = (bayer[2] + bayer[width * 2 + 2] + 1) >> 1;
            if (blue_line) 
            {
                *bgr++ = t0;
                *bgr++ = bayer[width + 2];
                *bgr++ = bayer[width + 1];
            } 
            else 
            {
                *bgr++ = bayer[width + 1];
                *bgr++ = bayer[width + 2];
                *bgr++ = t0;
            }
            bayer++;
        } 
        else
        {
            /* write last pixel */
            t0 = (bayer[0] + bayer[width * 2] + 1) >> 1;
            t1 = (bayer[1] + bayer[width * 2 + 1] + bayer[width] + 1) / 3;
            if (blue_line) 
            {
                *bgr++ = t0;
                *bgr++ = t1;
                *bgr++ = bayer[width + 1];
            } 
            else 
            {
                *bgr++ = bayer[width + 1];
                *bgr++ = t1;
                *bgr++ = t0;
            }
        }
 
        /* skip 2 border pixels */
        bayer += 2;
 
        blue_line = !blue_line;
        start_with_green = !start_with_green;
    }
 
    /* render the last line */
    convert_border_bayer_line_to_bgr24(bayer + width, bayer, bgr, width,
        !start_with_green, !blue_line);
}
 
/*convert bayer raw data to rgb24
* args: 
*      pBay: pointer to buffer containing Raw bayer data data
*      pRGB24: pointer to buffer containing rgb24 data
*      width: picture width
*      height: picture height
*      pix_order: bayer pixel order (0=gb/rg   1=gr/bg  2=bg/gr  3=rg/bg)
*/
static void 
bayer_to_rgb24(uint8_t *pBay, uint8_t *pRGB24, int width, int height, int pix_order)
{
    switch (pix_order) 
    {
        /*conversion functions are build for bgr, by switching b and r lines we get rgb*/
        case 0: /* gbgbgb... | rgrgrg... (V4L2_PIX_FMT_SGBRG8)*/
            bayer_to_rgbbgr24(pBay, pRGB24, width, height, TRUE, FALSE);
            break;
        
        case 1: /* grgrgr... | bgbgbg... (V4L2_PIX_FMT_SGRBG8)*/
            bayer_to_rgbbgr24(pBay, pRGB24, width, height, TRUE, TRUE);
            break;
        
        case 2: /* bgbgbg... | grgrgr... (V4L2_PIX_FMT_SBGGR8)*/
            bayer_to_rgbbgr24(pBay, pRGB24, width, height, FALSE, FALSE);
            break;
        
        case 3: /* rgrgrg... ! gbgbgb... (V4L2_PIX_FMT_SRGGB8)*/
            bayer_to_rgbbgr24(pBay, pRGB24, width, height, FALSE, TRUE);
            break;
            
        default: /* default is 0*/
            bayer_to_rgbbgr24(pBay, pRGB24, width, height, TRUE, FALSE);
            break;
    }
}
 
static void
flip_bgr_image(uint8_t *bgr_buff, int width, int height)
{
    int i =0;
    /*alloc a temp buffer*/
    uint8_t *tmp_buff = calloc(width*height*3, 1);
    uint8_t *t1 = bgr_buff + (width*height*3); /*point to the end of buffer*/
 
    for(i=0;i<height;i++)
    {
        t1 -= width * 3; /*decrement a line*/
        memcpy(tmp_buff+(i*width*3), t1, width*3); /* copy the line to temp buffer*/
    }
    /*get the fliped buffer*/
    memcpy(bgr_buff, tmp_buff, height*width*3);
    free(tmp_buff);
}
 
static int 
SaveBPM(const char *Filename, int width, int height, int BitCount, uint8_t *ImagePix) 
{
    int ret=0;
    BITMAPFILEHEADER BmpFileh;
    BITMAPINFOHEADER BmpInfoh;
    uint32_t imgsize;
    FILE *fp;
 
    imgsize=width*height*BitCount/8;
 
    BmpFileh.bfType=0x4d42;//must be BM (x4d42) 
    /*Specifies the size, in bytes, of the bitmap file*/
    BmpFileh.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+imgsize;
    BmpFileh.bfReserved1=0; //Reserved; must be zero
    BmpFileh.bfReserved2=0; //Reserved; must be zero
    /*Specifies the offset, in bytes,                      */
    /*from the beginning of the BITMAPFILEHEADER structure */
    /* to the bitmap bits                                  */
    BmpFileh.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); 
 
    BmpInfoh.biSize=40;
    BmpInfoh.biWidth=width; 
    BmpInfoh.biHeight=height; 
    BmpInfoh.biPlanes=1; 
    BmpInfoh.biBitCount=BitCount; 
    BmpInfoh.biCompression=0;  
    BmpInfoh.biSizeImage=imgsize; 
    BmpInfoh.biXPelsPerMeter=0; 
    BmpInfoh.biYPelsPerMeter=0; 
    BmpInfoh.biClrUsed=0; 
    BmpInfoh.biClrImportant=0;
 
//    if ((fp = fopen(Filename,"wb"))!=NULL) 
if ((fp = fopen("1.bmp","wb"))!=NULL) 
    {    
        ret=fwrite(&BmpFileh, sizeof(BITMAPFILEHEADER), 1, fp);
        ret+=fwrite(&BmpInfoh, sizeof(BITMAPINFOHEADER),1,fp);
        ret+=fwrite(ImagePix,imgsize,1,fp);
        if (ret<3) ret=1;
        else ret=0;
 
        fflush(fp); 
        if(fsync(fileno(fp)) || fclose(fp))
        {
            perror("BMP ERROR - couldn't write to file");
            ret=1;
        }
    } 
    else 
    {
        ret=1;
        printf("ERROR: Could not open file %s for write \n",Filename);
    }
    return ret;
}
 
int main(int argc, char *argv[])
{
 
    if(argc < 5)
        printf("usage: %s filename.raw filename.bmp width height [pixel_size_in_bits] [pix_order]\n", argv[0]);
 
    char* in_filename = argv[1];
    char* out_filename = argv[2];
    
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    int pix_size = 8;
    int pix_order = 0;
    if(argc > 5) 
        pix_size = atoi(argv[5]);
    if(argc > 6)
        pix_order = atoi(argv[6]);
        
    if((pix_size != 8) && (pix_size != 10) && (pix_size != 16))
    {
        printf("requested %i bits per pixel but only 8 and 10 bit bayer (normal & expanded to 16bits) is supported\n", pix_size);
        return(-1);
    }
    
    /*round by excess*/
    size_t in_buff_size = ((width*height*pix_size)/8) + 0.49;
    uint8_t* in_buff = calloc(in_buff_size, 1);
    
    /*bgr buffer*/
    uint8_t* bgr_buff = NULL;
    
    FILE *fp;
//    if ((fp = fopen(in_filename,"rb"))!=NULL) { 
if ((fp = fopen("CF000915.IIQ","rb"))!=NULL) { 
        printf("opening %s\n", in_filename);
    } else {
        printf("couldn't open %s for read\n",in_filename);
        free(in_buff);
        return (-1);
    }
    
    size_t bytes_read = fread(in_buff, 1, in_buff_size, fp);
    printf("read %i bytes from file %s\n", bytes_read, in_filename);
    
    if(pix_size == 10)
    {
        /*alocate a temp buffer*/
        uint8_t *tmp_buff = calloc(width*height, 1);
        uint8_t *t1 = tmp_buff;
        uint8_t *t2 = in_buff;
        
        uint16_t pixel;
    
        /*process 4 pixel group (5 bytes)*/
        int i =0;
        for(i=0;i<=(int)in_buff_size;i+=5)
        {
            
            pixel = t2[0] | (t2[1] & 0x03)<<8;
            *t1++ = (uint8_t) (pixel >> 2);
            
            pixel = (t2[1] & 0xFC)>>2 | (t2[2] & 0x0F)<<6 ;
            *t1++ = (uint8_t) (pixel >> 2);
            
            pixel = (t2[2] & 0xF0)>>4 | (t2[3] & 0x3F)<<4;
            *t1++ = (uint8_t) (pixel >> 2);
            
            pixel = (t2[3] & 0xC0)>>6 | (t2[4])<<2;
            *t1++ = (uint8_t) (pixel >> 2);
          
            t2+=5;
        }
        
        memcpy(in_buff, tmp_buff, width*height);
        free(tmp_buff);
    } else if(pix_size == 16) {
        /*alocate a temp buffer*/
        uint8_t *tmp_buff = calloc(width * height, 1);
        uint8_t *t1 = tmp_buff;
        uint8_t *t2 = in_buff;
        uint16_t pixel;
        int i = 0;
 
        /* Be lazy and convert from 10-bits to 8-bits bayer */
        for(i = 0; i <= (int)in_buff_size; i += 2) {
            pixel = t2[0] | (t2[1] & 0x03) << 8;
            *t1++ = (uint8_t)(pixel >> 2);
 
            t2+=2;
        }
 
        memcpy(in_buff, tmp_buff, width * height);
        free(tmp_buff);
    }
    
    //convert to bgr
    bgr_buff = calloc(width*height*3, 1);
    
    bayer_to_rgb24(in_buff, bgr_buff, width, height, pix_order);
    
    flip_bgr_image(bgr_buff, width, height);
    SaveBPM(out_filename, width, height, 24, bgr_buff);
    
    fclose(fp);
    free(in_buff);
    free(bgr_buff);
    return 0;
}
