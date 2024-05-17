xmlXPtrMatchString(const xmlChar *string, xmlNodePtr start, int startindex,
	            xmlNodePtr *end, int *endindex) {
    xmlNodePtr cur;
    int pos;  
    int len;  
    int stringlen;  
    int match;

    if (string == NULL)
	return(-1);
    if (start == NULL)
	return(-1);
    if ((end == NULL) || (endindex == NULL))
	return(-1);
    cur = start;
    if (cur == NULL)
	return(-1);
    pos = startindex - 1;
    stringlen = xmlStrlen(string);

    while (stringlen > 0) {
	if ((cur == *end) && (pos + stringlen > *endindex))
	    return(0);

	if ((cur->type != XML_ELEMENT_NODE) && (cur->content != NULL)) {
	    len = xmlStrlen(cur->content);
	    if (len >= pos + stringlen) {
		match = (!xmlStrncmp(&cur->content[pos], string, stringlen));
		if (match) {
#ifdef DEBUG_RANGES
		    xmlGenericError(xmlGenericErrorContext,
			    "found range %d bytes at index %d of ->",
			    stringlen, pos + 1);
		    xmlDebugDumpString(stdout, cur->content);
		    xmlGenericError(xmlGenericErrorContext, "\n");
#endif
		    *end = cur;
		    *endindex = pos + stringlen;
		    return(1);
		} else {
		    return(0);
		}
	    } else {
                int sub = len - pos;
		match = (!xmlStrncmp(&cur->content[pos], string, sub));
		if (match) {
#ifdef DEBUG_RANGES
		    xmlGenericError(xmlGenericErrorContext,
			    "found subrange %d bytes at index %d of ->",
			    sub, pos + 1);
		    xmlDebugDumpString(stdout, cur->content);
		    xmlGenericError(xmlGenericErrorContext, "\n");
#endif
                    string = &string[sub];
		    stringlen -= sub;
		} else {
		    return(0);
		}
	    }
	}
	cur = xmlXPtrAdvanceNode(cur, NULL);
	if (cur == NULL)
	    return(0);
	pos = 0;
    }
    return(1);
}
