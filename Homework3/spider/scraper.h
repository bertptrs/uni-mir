// Get a webpage as string
char* GetWebPage(const char* myurl);

// Get all links as a string
char* GetLinksFromWebPage(char* htmlcontent, char* url);
char* GetImageLinksFromWebPage(char* htmlcontent, char* url);
