xmlSplitQName(xmlParserCtxtPtr ctxt, const xmlChar *name, xmlChar **prefix) {
    xmlChar buf[XML_MAX_NAMELEN + 5];
    xmlChar *buffer = NULL;
 int len = 0;
 int max = XML_MAX_NAMELEN;
    xmlChar *ret = NULL;
 const xmlChar *cur = name;
 int c;

 if (prefix == NULL) return(NULL);
 *prefix = NULL;

 if (cur == NULL) return(NULL);

#ifndef XML_XML_NAMESPACE
  
 if ((cur[0] == 'x') && (cur[1] == 'm') &&
 (cur[2] == 'l') && (cur[3] == ':'))
 return(xmlStrdup(name));
#endif

  
 if (cur[0] == ':')
 return(xmlStrdup(name));

    c = *cur++;
 while ((c != 0) && (c != ':') && (len < max)) {  
	buf[len++] = c;
	c = *cur++;
 }
 if (len >= max) {
  
	max = len * 2;

	buffer = (xmlChar *) xmlMallocAtomic(max * sizeof(xmlChar));
 if (buffer == NULL) {
	    xmlErrMemory(ctxt, NULL);
 return(NULL);
 }
	memcpy(buffer, buf, len);
 while ((c != 0) && (c != ':')) {  
 if (len + 10 > max) {
	        xmlChar *tmp;

		max *= 2;
		tmp = (xmlChar *) xmlRealloc(buffer,
						max * sizeof(xmlChar));
 if (tmp == NULL) {
		    xmlFree(buffer);
		    xmlErrMemory(ctxt, NULL);
 return(NULL);
 }
		buffer = tmp;
 }
	    buffer[len++] = c;
	    c = *cur++;
 }
	buffer[len] = 0;
 }

 if ((c == ':') && (*cur == 0)) {
 if (buffer != NULL)
	    xmlFree(buffer);
 *prefix = NULL;
 return(xmlStrdup(name));
 }

 if (buffer == NULL)
	ret = xmlStrndup(buf, len);
 else {
	ret = buffer;
	buffer = NULL;
	max = XML_MAX_NAMELEN;
 }


 if (c == ':') {
	c = *cur;
 *prefix = ret;
 if (c == 0) {
 return(xmlStrndup(BAD_CAST "", 0));
 }
	len = 0;

  
 if (!(((c >= 0x61) && (c <= 0x7A)) ||
 ((c >= 0x41) && (c <= 0x5A)) ||
 (c == '_') || (c == ':'))) {
 int l;
 int first = CUR_SCHAR(cur, l);

 if (!IS_LETTER(first) && (first != '_')) {
		xmlFatalErrMsgStr(ctxt, XML_NS_ERR_QNAME,
 "Name %s is not XML Namespace compliant\n",
				  name);
 }
 }
	cur++;

 while ((c != 0) && (len < max)) {  
	    buf[len++] = c;
	    c = *cur++;
 }
 if (len >= max) {
  
	    max = len * 2;

	    buffer = (xmlChar *) xmlMallocAtomic(max * sizeof(xmlChar));
 if (buffer == NULL) {
	        xmlErrMemory(ctxt, NULL);
 return(NULL);
 }
	    memcpy(buffer, buf, len);
 while (c != 0) {  
 if (len + 10 > max) {
		    xmlChar *tmp;

		    max *= 2;
		    tmp = (xmlChar *) xmlRealloc(buffer,
						    max * sizeof(xmlChar));
 if (tmp == NULL) {
			xmlErrMemory(ctxt, NULL);
			xmlFree(buffer);
 return(NULL);
 }
		    buffer = tmp;
 }
		buffer[len++] = c;
		c = *cur++;
 }
	    buffer[len] = 0;
 }

 if (buffer == NULL)
	    ret = xmlStrndup(buf, len);
 else {
	    ret = buffer;
 }
 }

 return(ret);
}
