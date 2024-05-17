htmlParseElement(htmlParserCtxtPtr ctxt) {
    const xmlChar *name;
    xmlChar *currentNode = NULL;
    const htmlElemDesc * info;
    htmlParserNodeInfo node_info;
    int failed;
    int depth;
    const xmlChar *oldptr;

    if ((ctxt == NULL) || (ctxt->input == NULL)) {
	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		     "htmlParseElement: context error\n", NULL, NULL);
	return;
    }

    if (ctxt->instate == XML_PARSER_EOF)
        return;

     
    if (ctxt->record_info) {
        node_info.begin_pos = ctxt->input->consumed +
                          (CUR_PTR - ctxt->input->base);
	node_info.begin_line = ctxt->input->line;
    }

    failed = htmlParseStartTag(ctxt);
    name = ctxt->name;
    if ((failed == -1) || (name == NULL)) {
	if (CUR == '>')
	    NEXT;
        return;
    }

     
    info = htmlTagLookup(name);
    if (info == NULL) {
	htmlParseErr(ctxt, XML_HTML_UNKNOWN_TAG,
	             "Tag %s invalid\n", name, NULL);
    }

     
    if ((CUR == '/') && (NXT(1) == '>')) {
        SKIP(2);
	if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
	    ctxt->sax->endElement(ctxt->userData, name);
	htmlnamePop(ctxt);
	return;
    }

    if (CUR == '>') {
        NEXT;
    } else {
	htmlParseErr(ctxt, XML_ERR_GT_REQUIRED,
	             "Couldn't find end of Start Tag %s\n", name, NULL);

	 
	if (xmlStrEqual(name, ctxt->name)) {
	    nodePop(ctxt);
	    htmlnamePop(ctxt);
	}

	 
	if (ctxt->record_info) {
	   node_info.end_pos = ctxt->input->consumed +
			      (CUR_PTR - ctxt->input->base);
	   node_info.end_line = ctxt->input->line;
	   node_info.node = ctxt->node;
	   xmlParserAddNodeInfo(ctxt, &node_info);
	}
	return;
    }

     
    if ((info != NULL) && (info->empty)) {
	if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
	    ctxt->sax->endElement(ctxt->userData, name);
	htmlnamePop(ctxt);
	return;
    }

     
    currentNode = xmlStrdup(ctxt->name);
    depth = ctxt->nameNr;
    while (IS_CHAR_CH(CUR)) {
	oldptr = ctxt->input->cur;
	htmlParseContent(ctxt);
	if (oldptr==ctxt->input->cur) break;
	if (ctxt->nameNr < depth) break;
    }

     
    if ( currentNode != NULL && ctxt->record_info ) {
       node_info.end_pos = ctxt->input->consumed +
                          (CUR_PTR - ctxt->input->base);
       node_info.end_line = ctxt->input->line;
       node_info.node = ctxt->node;
       xmlParserAddNodeInfo(ctxt, &node_info);
    }
    if (!IS_CHAR_CH(CUR)) {
	htmlAutoCloseOnEnd(ctxt);
    }

    if (currentNode != NULL)
	xmlFree(currentNode);
}
