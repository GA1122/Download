exsltDateSumFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlNodeSetPtr ns;
    void *user = NULL;
    xmlChar *tmp;
    exsltDateValPtr x, total;
    xmlChar *ret;
    int i;

    if (nargs != 1) {
	xmlXPathSetArityError (ctxt);
	return;
    }

     
    if ((ctxt->value != NULL) && ctxt->value->boolval != 0) {
	user = ctxt->value->user;
	ctxt->value->boolval = 0;
	ctxt->value->user = NULL;
    }

    ns = xmlXPathPopNodeSet (ctxt);
    if (xmlXPathCheckError (ctxt))
	return;

    if ((ns == NULL) || (ns->nodeNr == 0)) {
	xmlXPathReturnEmptyString (ctxt);
	if (ns != NULL)
	    xmlXPathFreeNodeSet (ns);
	return;
    }

    total = exsltDateCreateDate (XS_DURATION);
    if (total == NULL) {
        xmlXPathFreeNodeSet (ns);
        return;
    }

    for (i = 0; i < ns->nodeNr; i++) {
	int result;
	tmp = xmlXPathCastNodeToString (ns->nodeTab[i]);
	if (tmp == NULL) {
	    xmlXPathFreeNodeSet (ns);
	    exsltDateFreeDate (total);
	    return;
	}

	x = exsltDateParseDuration (tmp);
	if (x == NULL) {
	    xmlFree (tmp);
	    exsltDateFreeDate (total);
	    xmlXPathFreeNodeSet (ns);
	    xmlXPathReturnEmptyString (ctxt);
	    return;
	}

	result = _exsltDateAddDurCalc(total, total, x);

	exsltDateFreeDate (x);
	xmlFree (tmp);
	if (!result) {
	    exsltDateFreeDate (total);
	    xmlXPathFreeNodeSet (ns);
	    xmlXPathReturnEmptyString (ctxt);
	    return;
	}
    }

    ret = exsltDateFormatDuration (&(total->value.dur));
    exsltDateFreeDate (total);

    xmlXPathFreeNodeSet (ns);
    if (user != NULL)
	xmlFreeNodeList ((xmlNodePtr) user);

    if (ret == NULL)
	xmlXPathReturnEmptyString (ctxt);
    else
	xmlXPathReturnString (ctxt, ret);
}
