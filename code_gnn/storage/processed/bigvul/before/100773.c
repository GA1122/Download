xmlSkipBlankChars(xmlParserCtxtPtr ctxt) {
    int res = 0;

     
    if ((ctxt->inputNr == 1) && (ctxt->instate != XML_PARSER_DTD)) {
	const xmlChar *cur;
	 
	cur = ctxt->input->cur;
	while (IS_BLANK_CH(*cur)) {
	    if (*cur == '\n') {
		ctxt->input->line++; ctxt->input->col = 1;
	    }
	    cur++;
	    res++;
	    if (*cur == 0) {
		ctxt->input->cur = cur;
		xmlParserInputGrow(ctxt->input, INPUT_CHUNK);
		cur = ctxt->input->cur;
	    }
	}
	ctxt->input->cur = cur;
    } else {
	int cur;
	do {
	    cur = CUR;
	    while (IS_BLANK_CH(cur)) {  
		NEXT;
		cur = CUR;
		res++;
	    }
	    while ((cur == 0) && (ctxt->inputNr > 1) &&
		   (ctxt->instate != XML_PARSER_COMMENT)) {
		xmlPopInput(ctxt);
		cur = CUR;
	    }
	     
	    if (*ctxt->input->cur == '%') xmlParserHandlePEReference(ctxt);
	} while (IS_BLANK(cur));  
    }
    return(res);
}
