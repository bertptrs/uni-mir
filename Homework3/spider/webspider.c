#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "scraper.h"
#include "htmlstreamparser.h"

#define MAXQSIZE 9000000       // Maximum size of the queue, q
#define MAXURL 100000          // Maximum size of a URL
#define MAXPAGESIZE 20000          // Maximum size of a webpage
#define MAXDOWNLOADS 15      // Maximum number of downloads we will attempt

int QSize(char *q)
	//
	//  This function counts the number of links in the queue
	//
{
	int k, total;
	total = 0;
	for(k=0;k<MAXQSIZE;k++)
	{
		if(q[k] == '\n') {total++;}
		if(q[k] == '\0') {return(total);}
	}
	return(total);
}
void AppendLinks(char *p, char *q, char *weblinks)
	//
	//  This functions appends weblinks to the end of q
	//  Whether you use a function from the string.h library
	//  or just write a 2 line for loop for copying weblinks to
	//  the end of q is fine.  Remember that each weblink
	//  should be separated by '\n' and q should end with '\0'
	//
	//  Feel free to use string functions like strcat, just make sure
	//  you check for memory borders properly
	//
	//  It is important to add code to check for the end of q (MAXQSIZE)
	//  to avoid segmentation faults.
	//
{
	// Need
	const size_t needed = strlen(weblinks) + 1;
	const size_t available = MAXQSIZE - (p - q) - strlen(p);
	if (available < needed) {
		puts("Reached end of queue... exiting.");
		free(q);
		exit(0);
	}

	strcat(p, weblinks);
}

int GetNextURL(char *p, char *q, char *myurl)
	//
	//  This function puts the next URL from p into myurl.
	//  We also pass the queue so that we can check for the end of the queue
	//
{
	int k;
	for(k=0;k<MAXURL;k++)
	{
		if(p[k]=='\n')
		{
			myurl[k] = '\0';
			return(1);
		}
		else {myurl[k] = p[k];}
	}
	//
	// If no URL is found, then return localhost in myurl and a zero from the function
	// This is to ensure that this function always returns an http URL
	//
	strcpy(myurl,"http://127.0.0.1");
	return(0);
}
char *ShiftP(char *p, char *q)
	//
	//  This function returns the shifted position of p to the next URL
	//  We also pass the queue so that we can check for the end of the queue
	//
{
	return strchr(p, '\n') + 1;
}

int main(int argc, char* argv[])
	//
	//  This code was written with speed and to some extent clarity in mind.  As you
	//  are supposed to be able to convert your own functions from the previous assignment
	//  to a WWW robot in under 2 hours, we also did a fast implementation to double-check
	//  the time required.
	//
	//  This basic WWW robot is using the two functions specified in class.
	//  html = GetWebPage(url);
	//  weblinks = GetLinksFromWebPage(html, url);
	//
	//  It does attempt to limit the downloads to Leiden University domains
	//  It does limit the total number of downloads
	//  It does not check for the Robot Exclusion Protocol: robots.txt because
	//  we are only looking at domains which belong to us.
	//
{
	char *url = NULL;
	char *p, *q;
	char urlspace[MAXURL];
	char *html, *weblinks, *imagelinks;
	int k, qs, ql;

	//
	// We will be putting all of the links found into q, short for "queue", and terminate with '\0'
	// Initially, it will just end at MAXQSIZE, however, we could easily turn it into a ring queue
	// where the end meets the beginning
	//
	q = ((char *) malloc(MAXQSIZE));  // When done, use free(q) to free the memory back to the OS
	if (q==NULL) {printf("\n\nProblem with malloc...exiting\n\n"); exit(0);}
	q[0]='\0';
	p = q;

	if (argc <= 1) {printf("\n\nNo webpage given...exiting\n\n"); exit(0);}
	else {
		url = argv[1];
		if(strstr(url,"http") != NULL) {printf("\nInitial web URL: %s\n\n", url);}
		else {printf("\n\nYou must start the URL with lowercase http...exiting\n\n"); exit(0);}
	}
	//
	//  Place the input URL into q and end it with '\n'
	//
	strcat(q, url);  strcat(q, "\n");
	url = urlspace;
	//
	//  The loop limitation, MAXDOWNLOADS is the maximum number of downloads we
	//  will allow the robot to perform.  It is just a precaution for this assignment
	//  to minimize runaway bots
	//
	for(k=0;k<MAXDOWNLOADS;k++)
	{
		qs = QSize(q); ql = strlen(q);
		printf("\nDownload #: %d   Weblinks: %d   Queue Size: %d\n",k, qs, ql);

		GetNextURL(p, q, url);
		ShiftP(p, q);
		puts(url);



		//
		//  This is a simple test to see if the url is within one of the allowed domains
		//  liacs.nl, leidenuniv.nl, leiden.edu, or mastersinleiden.nl.  It is
		//  not perfect but will probably work for most of the cases and was trivial to write.
		//
		if((strstr(url,"leidenuniv.nl") != NULL) || (strstr(url,"liacs.nl") != NULL))
		{
			//        printf("url=%s\n",url);
			html = GetWebPage(url);

			// You can adjust this error check depending on the rest of your code.  In this case
			// the downloaded webpage was NULL - had no html nor links
			if(html==NULL){printf("\n\nThe downloaded webpage was NULL\n\n");exit(0);}

			//        v = strlen(html); printf("\n\nwebpage size of %s is %d\n\n",url,v);
			if(html) {
				weblinks = GetLinksFromWebPage(html, url);
				AppendLinks(p, q, weblinks);
				free(html);
				free(weblinks);
			}
		}
		else {printf("\n\nNot in allowed domains: %s\n\n",url);}
	}
	FILE *fp;
	fp = fopen("mylinks.txt","w");
	if(fp != NULL) {fprintf(fp, "%s",q);}
	free(q);
}
