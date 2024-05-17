xmlXPtrEvalFullXPtr(xmlXPathParserContextPtr ctxt, xmlChar *name) {
    if (name == NULL)
    name = xmlXPathParseName(ctxt);
    if (name == NULL)
	XP_ERROR(XPATH_EXPR_ERROR);
    while (name != NULL) {
	ctxt->error = XPATH_EXPRESSION_OK;
	xmlXPtrEvalXPtrPart(ctxt, name);

	 
	if ((ctxt->error != XPATH_EXPRESSION_OK) &&
            (ctxt->error != XML_XPTR_UNKNOWN_SCHEME))
	    return;

	 
	if (ctxt->value != NULL) {
	    xmlXPathObjectPtr obj = ctxt->value;

	    switch (obj->type) {
		case XPATH_LOCATIONSET: {
		    xmlLocationSetPtr loc = ctxt->value->user;
		    if ((loc != NULL) && (loc->locNr > 0))
			return;
		    break;
		}
		case XPATH_NODESET: {
		    xmlNodeSetPtr loc = ctxt->value->nodesetval;
		    if ((loc != NULL) && (loc->nodeNr > 0))
			return;
		    break;
		}
		default:
		    break;
	    }

	     
	    do {
		obj = valuePop(ctxt);
		if (obj != NULL) {
		    xmlXPathFreeObject(obj);
		}
	    } while (obj != NULL);
	}

	 
	SKIP_BLANKS;
	name = xmlXPathParseName(ctxt);
    }
}
