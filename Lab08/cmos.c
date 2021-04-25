#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/fcntl.h> 
#include "asm-arm/arch-pxa/lib/creator_pxa270_cmos.h" 
main() 
{ 
 int terminate = 1; 
 int dwTotalReadSize, dwImageSize, nRead; 
 int count; 
 int fd_cmos; 
 cmos_info_t CMOSInfo; 
 unsigned char *pImageBuffer=NULL; 
 
 dwImageSize = 160*120; 
 pImageBuffer = malloc(dwImageSize); 
 fd_cmos = open("/dev/cmos",O_RDWR); 
 CMOSInfo.ImageSizeMode = CMOS_SIZE_MODE_160_120; 
 CMOSInfo.HighRef = 120; 
 ioctl(fd_cmos, CMOS_PARAMETER, &CMOSInfo); 
ioctl(fd_cmos, CMOS_ON, &CMOSInfo); 
 while (terminate) 
 { 
  if (ioctl(fd_cmos, CMOS_GET_STATUS, &CMOSInfo) >= 0) 
      if (CMOSInfo.Status == CMOS_IMG_READY) 
      { 
          dwTotalReadSize = 0; 
          count = dwImageSize; 
          do 
{ 
         if (count + dwTotalReadSize > dwImageSize) 
		 eSize - dwTotalReadSize; 
           if ((nRead = read(fd_cmos, pImageBuffer + dwTotalReadSize, count)) > 0)  
               dwTotalReadSize += nRead; 
           else if (nRead == 0) 
               break; /* EOF */ 
          }  
          while (dwTotalReadSize < dwImageSize); 
          /* now your image raw data is in the buffer. */ 
   } 
} 
  
if (pImageBuffer) 
  free(pImageBuffer); 
ioctl(fd_cmos, CMOS_OFF, &CMOSInfo);  
 close(fd_cmos); 
}
