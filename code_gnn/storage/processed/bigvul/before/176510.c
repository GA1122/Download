xmlCanonicPath(const xmlChar *path)
{
 
#if defined(_WIN32) && !defined(__CYGWIN__)
 int len = 0;
 int i = 0;
    xmlChar *p = NULL;
#endif
    xmlURIPtr uri;
    xmlChar *ret;
 const xmlChar *absuri;

 if (path == NULL)
 return(NULL);

#if defined(_WIN32)
  
 if ((path[0] == '\\') && (path[1] == '\\') && (path[2] == '?') &&
 (path[3] == '\\') )
 return xmlStrdup((const xmlChar *) path);
#endif

  
 if ((path[0] == '/') && (path[1] == '/') && (path[2] != '/'))
        path++;

 if ((uri = xmlParseURI((const char *) path)) != NULL) {
	xmlFreeURI(uri);
 return xmlStrdup(path);
 }

  
    absuri = xmlStrstr(path, BAD_CAST "://");
 if (absuri != NULL) {
 int l, j;
 unsigned char c;
	xmlChar *escURI;

  
	l = absuri - path;
  
 if ((l <= 0) || (l > 20))
 goto path_processing;
  
 for (j = 0;j < l;j++) {
	    c = path[j];
 if (!(((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))))
 goto path_processing;
 }

  
        escURI = xmlURIEscapeStr(path, BAD_CAST ":/?_.#&;=");
 if (escURI != NULL) {
  
	    uri = xmlParseURI((const char *) escURI);
  
 if (uri != NULL) {
	        xmlFreeURI(uri);
 return escURI;
 }
 }
 }

path_processing:
 
#if defined(_WIN32) && !defined(__CYGWIN__)
  
    uri = xmlCreateURI();
 if (uri == NULL) {  
 return(NULL);
 }

    len = xmlStrlen(path);
 if ((len > 2) && IS_WINDOWS_PATH(path)) {
  
	uri->scheme = xmlStrdup(BAD_CAST "file");
  
	uri->path = xmlMallocAtomic(len + 2);
 if (uri->path == NULL) {
	    xmlFreeURI(uri);  
 return(NULL);
 }
  
	uri->path[0] = '/';
	p = uri->path + 1;
	strncpy(p, path, len + 1);
 } else {
	uri->path = xmlStrdup(path);
 if (uri->path == NULL) {
	    xmlFreeURI(uri);
 return(NULL);
 }
	p = uri->path;
 }
  
 while (*p != '\0') {
 if (*p == '\\')
 *p = '/';
	p++;
 }

 if (uri->scheme == NULL) {
	ret = xmlStrdup((const xmlChar *) uri->path);
 } else {
	ret = xmlSaveUri(uri);
 }

    xmlFreeURI(uri);
#else
    ret = xmlStrdup((const xmlChar *) path);
#endif
 return(ret);
}
