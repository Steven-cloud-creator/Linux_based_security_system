#include <stdio.h>

int main()
{
	printf("Content-type:text/html\n\n");
	printf("<html><body>");
	printf("<font style=\"color:red; font-size:30px;\">Hello,CGI!</font><br/>");
	printf("<a href=\"/index.html\">return index.html</a>");
	printf("</body></html>");
	return 0;
}
