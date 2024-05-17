htmlParseDocument(htmlParserCtxtPtr ctxt) {
    xmlChar start[4];
    xmlCharEncoding enc;
    xmlDtdPtr dtd;

    xmlInitParser();

    htmlDefaultSAXHandlerInit();

    if ((ctxt == NULL) || (ctxt->input == NULL)) {
	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		     "htmlParseDocument: context error\n", NULL, NULL);
	return(XML_ERR_INTERNAL_ERROR);
    }
    ctxt->html = 1;
    ctxt->linenumbers = 1;
    GROW;
     
    if ((ctxt->sax) && (ctxt->sax->setDocumentLocator))
        ctxt->sax->setDocumentLocator(ctxt->userData, &xmlDefaultSAXLocator);

    if ((ctxt->encoding == (const xmlChar *)XML_CHAR_ENCODING_NONE) &&
        ((ctxt->input->end - ctxt->input->cur) >= 4)) {
	 
	start[0] = RAW;
	start[1] = NXT(1);
	start[2] = NXT(2);
	start[3] = NXT(3);
	enc = xmlDetectCharEncoding(&start[0], 4);
	if (enc != XML_CHAR_ENCODING_NONE) {
	    xmlSwitchEncoding(ctxt, enc);
	}
    }

     
    SKIP_BLANKS;
    if (CUR == 0) {
	htmlParseErr(ctxt, XML_ERR_DOCUMENT_EMPTY,
	             "Document is empty\n", NULL, NULL);
    }

    if ((ctxt->sax) && (ctxt->sax->startDocument) && (!ctxt->disableSAX))
	ctxt->sax->startDocument(ctxt->userData);


     
    while (((CUR == '<') && (NXT(1) == '!') &&
            (NXT(2) == '-') && (NXT(3) == '-')) ||
	   ((CUR == '<') && (NXT(1) == '?'))) {
        htmlParseComment(ctxt);
        htmlParsePI(ctxt);
	SKIP_BLANKS;
    }


     
    if ((CUR == '<') && (NXT(1) == '!') &&
	(UPP(2) == 'D') && (UPP(3) == 'O') &&
	(UPP(4) == 'C') && (UPP(5) == 'T') &&
	(UPP(6) == 'Y') && (UPP(7) == 'P') &&
	(UPP(8) == 'E')) {
	htmlParseDocTypeDecl(ctxt);
    }
    SKIP_BLANKS;

     
    while (((CUR == '<') && (NXT(1) == '!') &&
            (NXT(2) == '-') && (NXT(3) == '-')) ||
	   ((CUR == '<') && (NXT(1) == '?'))) {
        htmlParseComment(ctxt);
        htmlParsePI(ctxt);
	SKIP_BLANKS;
    }

     
    htmlParseContentInternal(ctxt);

     
    if (CUR == 0)
	htmlAutoCloseOnEnd(ctxt);


     
    if ((ctxt->sax) && (ctxt->sax->endDocument != NULL))
        ctxt->sax->endDocument(ctxt->userData);

    if ((!(ctxt->options & HTML_PARSE_NODEFDTD)) && (ctxt->myDoc != NULL)) {
	dtd = xmlGetIntSubset(ctxt->myDoc);
	if (dtd == NULL)
	    ctxt->myDoc->intSubset =
		xmlCreateIntSubset(ctxt->myDoc, BAD_CAST "html",
		    BAD_CAST "-//W3C//DTD HTML 4.0 Transitional//EN",
		    BAD_CAST "http://www.w3.org/TR/REC-html40/loose.dtd");
    }
    if (! ctxt->wellFormed) return(-1);
    return(0);
}