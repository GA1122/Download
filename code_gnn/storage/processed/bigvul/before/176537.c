xmlURIEscapeStr(const xmlChar *str, const xmlChar *list) {
    xmlChar *ret, ch;
    xmlChar *temp;
 const xmlChar *in;
 int len, out;

 if (str == NULL)
 return(NULL);
 if (str[0] == 0)
 return(xmlStrdup(str));
    len = xmlStrlen(str);
 if (!(len > 0)) return(NULL);

    len += 20;
    ret = (xmlChar *) xmlMallocAtomic(len);
 if (ret == NULL) {
        xmlURIErrMemory("escaping URI value\n");
 return(NULL);
 }
    in = (const xmlChar *) str;
    out = 0;
 while(*in != 0) {
 if (len - out <= 3) {
            temp = xmlSaveUriRealloc(ret, &len);
 if (temp == NULL) {
                xmlURIErrMemory("escaping URI value\n");
		xmlFree(ret);
 return(NULL);
 }
	    ret = temp;
 }

	ch = *in;

 if ((ch != '@') && (!IS_UNRESERVED(ch)) && (!xmlStrchr(list, ch))) {
 unsigned char val;
	    ret[out++] = '%';
	    val = ch >> 4;
 if (val <= 9)
		ret[out++] = '0' + val;
 else
		ret[out++] = 'A' + val - 0xA;
	    val = ch & 0xF;
 if (val <= 9)
		ret[out++] = '0' + val;
 else
		ret[out++] = 'A' + val - 0xA;
	    in++;
 } else {
	    ret[out++] = *in++;
 }

 }
    ret[out] = 0;
 return(ret);
}
