 xmlParseDocument(xmlParserCtxtPtr ctxt) {
     xmlChar start[4];
     xmlCharEncoding enc;
 
     xmlInitParser();
 
     if ((ctxt == NULL) || (ctxt->input == NULL))
         return(-1);
 
     GROW;
 
      
     xmlDetectSAX2(ctxt);
 
      
      if ((ctxt->sax) && (ctxt->sax->setDocumentLocator))
          ctxt->sax->setDocumentLocator(ctxt->userData, &xmlDefaultSAXLocator);
//     if (ctxt->instate == XML_PARSER_EOF)
// 	return(-1);
  
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
 
 
     if (CUR == 0) {
 	xmlFatalErr(ctxt, XML_ERR_DOCUMENT_EMPTY, NULL);
     }
 
      
     if ((ctxt->input->end - ctxt->input->cur) < 35) {
        GROW;
     }
     if ((CMP5(CUR_PTR, '<', '?', 'x', 'm', 'l')) && (IS_BLANK_CH(NXT(5)))) {
 
 	 
 	xmlParseXMLDecl(ctxt);
 	if (ctxt->errNo == XML_ERR_UNSUPPORTED_ENCODING) {
 	     
 	    return(-1);
 	}
 	ctxt->standalone = ctxt->input->standalone;
 	SKIP_BLANKS;
     } else {
 	ctxt->version = xmlCharStrdup(XML_DEFAULT_VERSION);
      }
      if ((ctxt->sax) && (ctxt->sax->startDocument) && (!ctxt->disableSAX))
          ctxt->sax->startDocument(ctxt->userData);
//     if (ctxt->instate == XML_PARSER_EOF)
// 	return(-1);
  
       
     GROW;
     xmlParseMisc(ctxt);
 
      
     GROW;
     if (CMP9(CUR_PTR, '<', '!', 'D', 'O', 'C', 'T', 'Y', 'P', 'E')) {
 
 	ctxt->inSubset = 1;
 	xmlParseDocTypeDecl(ctxt);
  	if (RAW == '[') {
  	    ctxt->instate = XML_PARSER_DTD;
  	    xmlParseInternalSubset(ctxt);
// 	    if (ctxt->instate == XML_PARSER_EOF)
// 		return(-1);
  	}
  
  	 
 	ctxt->inSubset = 2;
 	if ((ctxt->sax != NULL) && (ctxt->sax->externalSubset != NULL) &&
  	    (!ctxt->disableSAX))
  	    ctxt->sax->externalSubset(ctxt->userData, ctxt->intSubName,
  	                              ctxt->extSubSystem, ctxt->extSubURI);
// 	if (ctxt->instate == XML_PARSER_EOF)
// 	    return(-1);
  	ctxt->inSubset = 0;
  
          xmlCleanSpecialAttr(ctxt);
 
 	ctxt->instate = XML_PARSER_PROLOG;
 	xmlParseMisc(ctxt);
     }
 
      
     GROW;
     if (RAW != '<') {
 	xmlFatalErrMsg(ctxt, XML_ERR_DOCUMENT_EMPTY,
 		       "Start tag expected, '<' not found\n");
     } else {
 	ctxt->instate = XML_PARSER_CONTENT;
 	xmlParseElement(ctxt);
 	ctxt->instate = XML_PARSER_EPILOG;
 
 
 	 
 	xmlParseMisc(ctxt);
 
 	if (RAW != 0) {
 	    xmlFatalErr(ctxt, XML_ERR_DOCUMENT_END, NULL);
 	}
 	ctxt->instate = XML_PARSER_EOF;
     }
 
      
     if ((ctxt->sax) && (ctxt->sax->endDocument != NULL))
         ctxt->sax->endDocument(ctxt->userData);
 
      
     if ((ctxt->myDoc != NULL) &&
 	(xmlStrEqual(ctxt->myDoc->version, SAX_COMPAT_MODE))) {
 	xmlFreeDoc(ctxt->myDoc);
 	ctxt->myDoc = NULL;
     }
 
     if ((ctxt->wellFormed) && (ctxt->myDoc != NULL)) {
         ctxt->myDoc->properties |= XML_DOC_WELLFORMED;
 	if (ctxt->valid)
 	    ctxt->myDoc->properties |= XML_DOC_DTDVALID;
 	if (ctxt->nsWellFormed)
 	    ctxt->myDoc->properties |= XML_DOC_NSVALID;
 	if (ctxt->options & XML_PARSE_OLD10)
 	    ctxt->myDoc->properties |= XML_DOC_OLD10;
     }
     if (! ctxt->wellFormed) {
 	ctxt->valid = 0;
 	return(-1);
     }
     return(0);
 }