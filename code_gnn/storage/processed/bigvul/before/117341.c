xmlXPtrSearchString(const xmlChar *string, xmlNodePtr *start, int *startindex,
	            xmlNodePtr *end, int *endindex) {
    xmlNodePtr cur;
    const xmlChar *str;
    int pos;  
    int len;  
    xmlChar first;

    if (string == NULL)
	return(-1);
    if ((start == NULL) || (startindex == NULL))
	return(-1);
    if ((end == NULL) || (endindex == NULL))
	return(-1);
    cur = *start;
    if (cur == NULL)
	return(-1);
    pos = *startindex - 1;
    first = string[0];

    while (cur != NULL) {
	if ((cur->type != XML_ELEMENT_NODE) && (cur->content != NULL)) {
	    len = xmlStrlen(cur->content);
	    while (pos <= len) {
		if (first != 0) {
		    str = xmlStrchr(&cur->content[pos], first);
		    if (str != NULL) {
			pos = (str - (xmlChar *)(cur->content));
#ifdef DEBUG_RANGES
			xmlGenericError(xmlGenericErrorContext,
				"found '%c' at index %d of ->",
				first, pos + 1);
			xmlDebugDumpString(stdout, cur->content);
			xmlGenericError(xmlGenericErrorContext, "\n");
#endif
			if (xmlXPtrMatchString(string, cur, pos + 1,
					       end, endindex)) {
			    *start = cur;
			    *startindex = pos + 1;
			    return(1);
			}
			pos++;
		    } else {
			pos = len + 1;
		    }
		} else {
		     
#ifdef DEBUG_RANGES
		    xmlGenericError(xmlGenericErrorContext,
			    "found '' at index %d of ->",
			    pos + 1);
		    xmlDebugDumpString(stdout, cur->content);
		    xmlGenericError(xmlGenericErrorContext, "\n");
#endif
		    *start = cur;
		    *startindex = pos + 1;
		    *end = cur;
		    *endindex = pos + 1;
		    return(1);
		}
	    }
	}
	if ((cur == *end) && (pos >= *endindex))
	    return(0);
	cur = xmlXPtrAdvanceNode(cur, NULL);
	if (cur == NULL)
	    return(0);
	pos = 1;
    }
    return(0);
}
