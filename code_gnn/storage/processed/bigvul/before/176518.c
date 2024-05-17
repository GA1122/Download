xmlParse3986HierPart(xmlURIPtr uri, const char **str)
{
 const char *cur;
 int ret;

    cur = *str;

 if ((*cur == '/') && (*(cur + 1) == '/')) {
        cur += 2;
	ret = xmlParse3986Authority(uri, &cur);
 if (ret != 0) return(ret);
 if (uri->server == NULL)
	    uri->port = -1;
	ret = xmlParse3986PathAbEmpty(uri, &cur);
 if (ret != 0) return(ret);
 *str = cur;
 return(0);
 } else if (*cur == '/') {
        ret = xmlParse3986PathAbsolute(uri, &cur);
 if (ret != 0) return(ret);
 } else if (ISA_PCHAR(cur)) {
        ret = xmlParse3986PathRootless(uri, &cur);
 if (ret != 0) return(ret);
 } else {
  
 if (uri != NULL) {
 if (uri->path != NULL) xmlFree(uri->path);
	    uri->path = NULL;
 }
 }
 *str = cur;
 return (0);
}
