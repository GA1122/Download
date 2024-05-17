xmlParse3986URI(xmlURIPtr uri, const char *str) {
 int ret;

    ret = xmlParse3986Scheme(uri, &str);
 if (ret != 0) return(ret);
 if (*str != ':') {
 return(1);
 }
    str++;
    ret = xmlParse3986HierPart(uri, &str);
 if (ret != 0) return(ret);
 if (*str == '?') {
	str++;
	ret = xmlParse3986Query(uri, &str);
 if (ret != 0) return(ret);
 }
 if (*str == '#') {
	str++;
	ret = xmlParse3986Fragment(uri, &str);
 if (ret != 0) return(ret);
 }
 if (*str != 0) {
	xmlCleanURI(uri);
 return(1);
 }
 return(0);
}
