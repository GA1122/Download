xmlXPathConcatFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur, newobj;
    xmlChar *tmp;

    if (ctxt == NULL) return;
    if (nargs < 2) {
	CHECK_ARITY(2);
    }

    CAST_TO_STRING;
    cur = valuePop(ctxt);
    if ((cur == NULL) || (cur->type != XPATH_STRING)) {
	xmlXPathReleaseObject(ctxt->context, cur);
	return;
    }
    nargs--;

    while (nargs > 0) {
	CAST_TO_STRING;
	newobj = valuePop(ctxt);
	if ((newobj == NULL) || (newobj->type != XPATH_STRING)) {
	    xmlXPathReleaseObject(ctxt->context, newobj);
	    xmlXPathReleaseObject(ctxt->context, cur);
	    XP_ERROR(XPATH_INVALID_TYPE);
	}
	tmp = xmlStrcat(newobj->stringval, cur->stringval);
	newobj->stringval = cur->stringval;
	cur->stringval = tmp;
	xmlXPathReleaseObject(ctxt->context, newobj);
	nargs--;
    }
    valuePush(ctxt, cur);
}
