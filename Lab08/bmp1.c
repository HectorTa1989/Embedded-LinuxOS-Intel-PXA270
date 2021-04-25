//#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include<sys/fcntl.h>
int main(int argc, char *argv[]){
	FILE *filestream, *filestream2;
	int pass, i, x, y, padded_xres, xres, yres;
	unsigned short int bpp, curint;
	unsigned long int biHeight, biWidth, curlong, imageoffset;
	unsigned char curchar, filename[14], filename2[14];

	puts("==============================");
	puts("     RAW TO BMP CONVERTER     ");
	puts("        Sam Palmer 2004       ");
	puts("==============================");

	printf("Enter filename: ");
	scanf("%s", filename);
	
	printf("Opening raw image file...");
//	if(!(filestream = fopen(filename, "rb"))){
	if(!(filestream = fopen("CF000915.IIQ", "rb"))){
		printf("error!\n");
//		getch();
		exit(1);
	}
	printf("done.\n");
	
	do{
		printf("Enter horizontal resolution (eg. 320): ");
		scanf("%d", &xres);
		printf("Enter vertical resolution (eg. 200): ");
		scanf("%d", &yres);
		
		fseek(filestream, 0, SEEK_END);
		if(ftell(filestream)!=xres*yres) puts("Filesize doesn't match the resolution!");
	}while(ftell(filestream)!=xres*yres);

	fseek(filestream, 0, SEEK_SET);
	
	padded_xres = xres;
	while(padded_xres%4!=0){
		padded_xres++;
	}

	strcpy(filename2, filename);
	filename2[strlen(filename2)-3] = '\0';
	strcat(filename2, "bmp");
	remove(filename2);
	
	printf("Creating %s ...", filename2);
	if(!(filestream2 = fopen(filename2, "wb"))){
		printf("error!\n");
//		getch();
		exit(1);
	}
	printf("done.\n");

	printf("Writing file header...");
	fputc('B', filestream2);
	fputc('M', filestream2);
	curlong = 0;
	fwrite(&curlong, 4, 1, filestream2);
	curint = 0;
	fwrite(&curint, 2, 1, filestream2);
	fwrite(&curint, 2, 1, filestream2);
	curlong = 1078;
	fwrite(&curlong, 4, 1, filestream2);
	printf("done.\n");
	
	printf("Writing info header...");
	curlong = 40;
	fwrite(&curlong, 4, 1, filestream2);
	curlong = xres;
	fwrite(&curlong, 4, 1, filestream2);
	curlong = yres;
	fwrite(&curlong, 4, 1, filestream2);
	curint = 1;
	fwrite(&curint, 2, 1, filestream2);
	curint = 8;
	fwrite(&curint, 2, 1, filestream2);
	curlong = 0;
	fwrite(&curlong, 4, 1, filestream2);
	curlong = padded_xres*yres;
	fwrite(&curlong, 4, 1, filestream2);
	curlong = 0;
	fwrite(&curlong, 4, 1, filestream2);
	fwrite(&curlong, 4, 1, filestream2);
	fwrite(&curlong, 4, 1, filestream2);
	fwrite(&curlong, 4, 1, filestream2);
	printf("done.\n");
	
	printf("Writing palette data...");
	for(i=0;i<256;i++){
		fputc(i, filestream2);
		fputc(i, filestream2);
		fputc(i, filestream2);
		fputc(0, filestream2);
	}
	printf("done.\n");

	printf("Writing image data...");
	for(y=0;y<yres;y++){
		fseek(filestream, (yres-1-y)*xres, SEEK_SET);
		for(x=0;x<padded_xres;x++){
			if(x>=xres) fputc(0, filestream2);
			else fputc(fgetc(filestream), filestream2);
		}
	}
	printf("done.\n");

	curlong = ftell(filestream2);
	fseek(filestream2, 2, SEEK_SET);
	fwrite(&curlong, 4, 1, filestream2);

	fclose(filestream);
	fclose(filestream2);

	puts("Conversion finished successfully!");

//	while(!kbhit());
//	getch();

	return 0;
}
