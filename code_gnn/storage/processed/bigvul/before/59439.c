xmlHaltParser(xmlParserCtxtPtr ctxt) {
    if (ctxt == NULL)
        return;
    ctxt->instate = XML_PARSER_EOF;
    ctxt->disableSAX = 1;
    while (ctxt->inputNr > 1)
        xmlFreeInputStream(inputPop(ctxt));
    if (ctxt->input != NULL) {
         
        if (ctxt->input->free != NULL) {
	    ctxt->input->free((xmlChar *) ctxt->input->base);
	    ctxt->input->free = NULL;
	}
	ctxt->input->cur = BAD_CAST"";
	ctxt->input->base = ctxt->input->cur;
        ctxt->input->end = ctxt->input->cur;
    }
}