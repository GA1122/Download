xmlSkipBlankChars(xmlParserCtxtPtr ctxt) {
    int res = 0;

     
    if ((ctxt->inputNr == 1) && (ctxt->instate != XML_PARSER_DTD)) {
	const xmlChar *cur;
	 
	cur = ctxt->input->cur;
	while (IS_BLANK_CH(*cur)) {
	    if (*cur == '\n') {
		ctxt->input->line++; ctxt->input->col = 1;
	    } else {
		ctxt->input->col++;
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
        int expandPE = ((ctxt->external != 0) || (ctxt->inputNr != 1));

	while (1) {
            if (IS_BLANK_CH(CUR)) {  
		NEXT;
	    } else if (CUR == '%') {
                 
	        if ((expandPE == 0) || (IS_BLANK_CH(NXT(1))) || (NXT(1) == 0))
                    break;
	        xmlParsePEReference(ctxt);
            } else if (CUR == 0) {
                if (ctxt->inputNr <= 1)
                    break;
                xmlPopInput(ctxt);
            } else {
                break;
            }

             
	    res++;
        }
    }
    return(res);
}
