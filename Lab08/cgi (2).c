#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	printf("Content-type:text/html\n\n");
	printf("<html>\n<body>\n\n");

	printf("Hello \n\n");
	printf("<img src = \"http://192.168.0.218/hung_cmos.bmp\">\n\n");
	printf("<img src = \"http://192.168.0.218/hung_cmos.jpg\">\n\n");

	printf("</body>\n</html>\n");
	
	return 0;
}
