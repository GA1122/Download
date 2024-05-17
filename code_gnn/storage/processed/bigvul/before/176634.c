xmlParseEncodingDecl(xmlParserCtxtPtr ctxt) {
    xmlChar *encoding = NULL;

    SKIP_BLANKS;
 if (CMP8(CUR_PTR, 'e', 'n', 'c', 'o', 'd', 'i', 'n', 'g')) {
	SKIP(8);
	SKIP_BLANKS;
 if (RAW != '=') {
	    xmlFatalErr(ctxt, XML_ERR_EQUAL_REQUIRED, NULL);
 return(NULL);
 }
	NEXT;
	SKIP_BLANKS;
 if (RAW == '"') {
	    NEXT;
	    encoding = xmlParseEncName(ctxt);
 if (RAW != '"') {
		xmlFatalErr(ctxt, XML_ERR_STRING_NOT_CLOSED, NULL);
		xmlFree((xmlChar *) encoding);
 return(NULL);
 } else
	        NEXT;
 } else if (RAW == '\''){
	    NEXT;
	    encoding = xmlParseEncName(ctxt);
 if (RAW != '\'') {
		xmlFatalErr(ctxt, XML_ERR_STRING_NOT_CLOSED, NULL);
		xmlFree((xmlChar *) encoding);
 return(NULL);
 } else
	        NEXT;
 } else {
	    xmlFatalErr(ctxt, XML_ERR_STRING_NOT_STARTED, NULL);
 }

  
 if (ctxt->options & XML_PARSE_IGNORE_ENC) {
	    xmlFree((xmlChar *) encoding);
 return(NULL);
 }

  
 if ((encoding != NULL) &&
 ((!xmlStrcasecmp(encoding, BAD_CAST "UTF-16")) ||
 (!xmlStrcasecmp(encoding, BAD_CAST "UTF16")))) {
  
 if ((ctxt->encoding == NULL) &&
 (ctxt->input->buf != NULL) &&
 (ctxt->input->buf->encoder == NULL)) {
		xmlFatalErrMsg(ctxt, XML_ERR_INVALID_ENCODING,
 "Document labelled UTF-16 but has UTF-8 content\n");
 }
 if (ctxt->encoding != NULL)
		xmlFree((xmlChar *) ctxt->encoding);
	    ctxt->encoding = encoding;
 }
  
 else if ((encoding != NULL) &&
 ((!xmlStrcasecmp(encoding, BAD_CAST "UTF-8")) ||
 (!xmlStrcasecmp(encoding, BAD_CAST "UTF8")))) {
 if (ctxt->encoding != NULL)
		xmlFree((xmlChar *) ctxt->encoding);
	    ctxt->encoding = encoding;
 }
 else if (encoding != NULL) {
	    xmlCharEncodingHandlerPtr handler;

 if (ctxt->input->encoding != NULL)
		xmlFree((xmlChar *) ctxt->input->encoding);
	    ctxt->input->encoding = encoding;

            handler = xmlFindCharEncodingHandler((const char *) encoding);
 if (handler != NULL) {
 if (xmlSwitchToEncoding(ctxt, handler) < 0) {
  
		    ctxt->errNo = XML_ERR_UNSUPPORTED_ENCODING;
 return(NULL);
 }
 } else {
		xmlFatalErrMsgStr(ctxt, XML_ERR_UNSUPPORTED_ENCODING,
 "Unsupported encoding %s\n", encoding);
 return(NULL);
 }
 }
 }
 return(encoding);
}
