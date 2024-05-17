xmlParseNCNameComplex(xmlParserCtxtPtr ctxt) {
 int len = 0, l;
 int c;
 int count = 0;
 size_t startPosition = 0;

#ifdef DEBUG
    nbParseNCNameComplex++;
#endif

  
    GROW;
    startPosition = CUR_PTR - BASE_PTR;
    c = CUR_CHAR(l);
 if ((c == ' ') || (c == '>') || (c == '/') ||  
 (!xmlIsNameStartChar(ctxt, c) || (c == ':'))) {
 return(NULL);
 }

 while ((c != ' ') && (c != '>') && (c != '/') &&  
 (xmlIsNameChar(ctxt, c) && (c != ':'))) {
 if (count++ > XML_PARSER_CHUNK_SIZE) {
 if ((len > XML_MAX_NAME_LENGTH) &&
 ((ctxt->options & XML_PARSE_HUGE) == 0)) {
                xmlFatalErr(ctxt, XML_ERR_NAME_TOO_LONG, "NCName");
 return(NULL);
 }
	    count = 0;
	    GROW;
 if (ctxt->instate == XML_PARSER_EOF)
 return(NULL);
 }
	len += l;
	NEXTL(l);
	c = CUR_CHAR(l);
 if (c == 0) {
	    count = 0;
  
	    ctxt->input->cur -= l;
	    GROW;
	    ctxt->input->cur += l;
 if (ctxt->instate == XML_PARSER_EOF)
 return(NULL);
	    c = CUR_CHAR(l);
 }
 }
 if ((len > XML_MAX_NAME_LENGTH) &&
 ((ctxt->options & XML_PARSE_HUGE) == 0)) {
        xmlFatalErr(ctxt, XML_ERR_NAME_TOO_LONG, "NCName");
 return(NULL);
 }
 return(xmlDictLookup(ctxt->dict, (BASE_PTR + startPosition), len));
}
