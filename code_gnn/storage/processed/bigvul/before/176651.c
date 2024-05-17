xmlParseInternalSubset(xmlParserCtxtPtr ctxt) {
  
 if (RAW == '[') {
        ctxt->instate = XML_PARSER_DTD;
        NEXT;
  
 while ((RAW != ']') && (ctxt->instate != XML_PARSER_EOF)) {
 const xmlChar *check = CUR_PTR;
 unsigned int cons = ctxt->input->consumed;

	    SKIP_BLANKS;
	    xmlParseMarkupDecl(ctxt);
	    xmlParsePEReference(ctxt);

  
 while ((RAW == 0) && (ctxt->inputNr > 1))
		xmlPopInput(ctxt);

 if ((CUR_PTR == check) && (cons == ctxt->input->consumed)) {
		xmlFatalErr(ctxt, XML_ERR_INTERNAL_ERROR,
 "xmlParseInternalSubset: error detected in Markup declaration\n");
 break;
 }
 }
 if (RAW == ']') {
	    NEXT;
	    SKIP_BLANKS;
 }
 }

  
 if (RAW != '>') {
	xmlFatalErr(ctxt, XML_ERR_DOCTYPE_NOT_FINISHED, NULL);
 return;
 }
    NEXT;
}
