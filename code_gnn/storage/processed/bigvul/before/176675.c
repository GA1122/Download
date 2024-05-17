xmlParseTextDecl(xmlParserCtxtPtr ctxt) {
    xmlChar *version;
 const xmlChar *encoding;

  
 if ((CMP5(CUR_PTR, '<', '?', 'x', 'm', 'l')) && (IS_BLANK_CH(NXT(5)))) {
	SKIP(5);
 } else {
	xmlFatalErr(ctxt, XML_ERR_XMLDECL_NOT_STARTED, NULL);
 return;
 }

 if (!IS_BLANK_CH(CUR)) {
	xmlFatalErrMsg(ctxt, XML_ERR_SPACE_REQUIRED,
 "Space needed after '<?xml'\n");
 }
    SKIP_BLANKS;

  
    version = xmlParseVersionInfo(ctxt);
 if (version == NULL)
	version = xmlCharStrdup(XML_DEFAULT_VERSION);
 else {
 if (!IS_BLANK_CH(CUR)) {
	    xmlFatalErrMsg(ctxt, XML_ERR_SPACE_REQUIRED,
 "Space needed here\n");
 }
 }
    ctxt->input->version = version;

  
    encoding = xmlParseEncodingDecl(ctxt);
 if (ctxt->errNo == XML_ERR_UNSUPPORTED_ENCODING) {
  
 return;
 }
 if ((encoding == NULL) && (ctxt->errNo == XML_ERR_OK)) {
	xmlFatalErrMsg(ctxt, XML_ERR_MISSING_ENCODING,
 "Missing encoding in text declaration\n");
 }

    SKIP_BLANKS;
 if ((RAW == '?') && (NXT(1) == '>')) {
        SKIP(2);
 } else if (RAW == '>') {
  
	xmlFatalErr(ctxt, XML_ERR_XMLDECL_NOT_FINISHED, NULL);
	NEXT;
 } else {
	xmlFatalErr(ctxt, XML_ERR_XMLDECL_NOT_FINISHED, NULL);
	MOVETO_ENDTAG(CUR_PTR);
	NEXT;
 }
}
