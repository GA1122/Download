xmlParse3986URIReference(xmlURIPtr uri, const char *str) {
 int ret;

 if (str == NULL)
 return(-1);
    xmlCleanURI(uri);

  
    ret = xmlParse3986URI(uri, str);
 if (ret != 0) {
	xmlCleanURI(uri);
        ret = xmlParse3986RelativeRef(uri, str);
 if (ret != 0) {
	    xmlCleanURI(uri);
 return(ret);
 }
 }
 return(0);
}
