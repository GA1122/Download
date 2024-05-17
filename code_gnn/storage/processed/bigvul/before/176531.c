xmlParseURI(const char *str) {
    xmlURIPtr uri;
 int ret;

 if (str == NULL)
 return(NULL);
    uri = xmlCreateURI();
 if (uri != NULL) {
	ret = xmlParse3986URIReference(uri, str);
 if (ret) {
	    xmlFreeURI(uri);
 return(NULL);
 }
 }
 return(uri);
}
