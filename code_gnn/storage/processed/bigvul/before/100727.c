xmlParseEntityValue(xmlParserCtxtPtr ctxt, xmlChar **orig) {
    xmlChar *buf = NULL;
    int len = 0;
    int size = XML_PARSER_BUFFER_SIZE;
    int c, l;
    xmlChar stop;
    xmlChar *ret = NULL;
    const xmlChar *cur = NULL;
    xmlParserInputPtr input;

    if (RAW == '"') stop = '"';
    else if (RAW == '\'') stop = '\'';
    else {
	xmlFatalErr(ctxt, XML_ERR_ENTITY_NOT_STARTED, NULL);
	return(NULL);
    }
    buf = (xmlChar *) xmlMallocAtomic(size * sizeof(xmlChar));
    if (buf == NULL) {
	xmlErrMemory(ctxt, NULL);
	return(NULL);
    }

     

    ctxt->instate = XML_PARSER_ENTITY_VALUE;
    input = ctxt->input;
    GROW;
    NEXT;
    c = CUR_CHAR(l);
     
    while ((IS_CHAR(c)) && ((c != stop) ||  
	   (ctxt->input != input))) {
	if (len + 5 >= size) {
	    xmlChar *tmp;

	    size *= 2;
	    tmp = (xmlChar *) xmlRealloc(buf, size * sizeof(xmlChar));
	    if (tmp == NULL) {
		xmlErrMemory(ctxt, NULL);
		xmlFree(buf);
		return(NULL);
	    }
	    buf = tmp;
	}
	COPY_BUF(l,buf,len,c);
	NEXTL(l);
	 
	while ((RAW == 0) && (ctxt->inputNr > 1))  
	    xmlPopInput(ctxt);

	GROW;
	c = CUR_CHAR(l);
	if (c == 0) {
	    GROW;
	    c = CUR_CHAR(l);
	}
    }
    buf[len] = 0;

     
    cur = buf;
    while (*cur != 0) {  
	if ((*cur == '%') || ((*cur == '&') && (cur[1] != '#'))) {
	    xmlChar *name;
	    xmlChar tmp = *cur;

	    cur++;
	    name = xmlParseStringName(ctxt, &cur);
            if ((name == NULL) || (*cur != ';')) {
		xmlFatalErrMsgInt(ctxt, XML_ERR_ENTITY_CHAR_ERROR,
	    "EntityValue: '%c' forbidden except for entities references\n",
	                          tmp);
	    }
	    if ((tmp == '%') && (ctxt->inSubset == 1) &&
		(ctxt->inputNr == 1)) {
		xmlFatalErr(ctxt, XML_ERR_ENTITY_PE_INTERNAL, NULL);
	    }
	    if (name != NULL)
		xmlFree(name);
	    if (*cur == 0)
	        break;
	}
	cur++;
    }

     
    if (c != stop) {
	xmlFatalErr(ctxt, XML_ERR_ENTITY_NOT_FINISHED, NULL);
	xmlFree(buf);
    } else {
	NEXT;
	 
	ret = xmlStringDecodeEntities(ctxt, buf, XML_SUBSTITUTE_PEREF,
				      0, 0, 0);
	if (orig != NULL) 
	    *orig = buf;
	else
	    xmlFree(buf);
    }
    
    return(ret);
}
