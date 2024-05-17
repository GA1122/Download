xmlParseEndTag1(xmlParserCtxtPtr ctxt, int line) {
 const xmlChar *name;

    GROW;
 if ((RAW != '<') || (NXT(1) != '/')) {
	xmlFatalErrMsg(ctxt, XML_ERR_LTSLASH_REQUIRED,
 "xmlParseEndTag: '</' not found\n");
 return;
 }
    SKIP(2);

    name = xmlParseNameAndCompare(ctxt,ctxt->name);

  
    GROW;
    SKIP_BLANKS;
 if ((!IS_BYTE_CHAR(RAW)) || (RAW != '>')) {
	xmlFatalErr(ctxt, XML_ERR_GT_REQUIRED, NULL);
 } else
	NEXT1;

  
 if (name != (xmlChar*)1) {
 if (name == NULL) name = BAD_CAST "unparseable";
        xmlFatalErrMsgStrIntStr(ctxt, XML_ERR_TAG_NAME_MISMATCH,
 "Opening and ending tag mismatch: %s line %d and %s\n",
		                ctxt->name, line, name);
 }

  
 if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL) &&
 (!ctxt->disableSAX))
        ctxt->sax->endElement(ctxt->userData, ctxt->name);

    namePop(ctxt);
    spacePop(ctxt);
 return;
}
