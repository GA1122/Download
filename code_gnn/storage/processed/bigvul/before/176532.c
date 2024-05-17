xmlParseURIRaw(const char *str, int raw) {
    xmlURIPtr uri;
 int ret;

 if (str == NULL)
 return(NULL);
    uri = xmlCreateURI();
 if (uri != NULL) {
 if (raw) {
	    uri->cleanup |= 2;
 }
	ret = xmlParseURIReference(uri, str);
 if (ret) {
	    xmlFreeURI(uri);
 return(NULL);
 }
 }
 return(uri);
}
