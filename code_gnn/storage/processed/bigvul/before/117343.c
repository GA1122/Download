xmlXPtrStringRangeFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    int i, startindex, endindex = 0, fendindex;
    xmlNodePtr start, end = 0, fend;
    xmlXPathObjectPtr set;
    xmlLocationSetPtr oldset;
    xmlLocationSetPtr newset;
    xmlXPathObjectPtr string;
    xmlXPathObjectPtr position = NULL;
    xmlXPathObjectPtr number = NULL;
    int found, pos = 0, num = 0;

     
    if ((nargs < 2) || (nargs > 4))
	XP_ERROR(XPATH_INVALID_ARITY);

    if (nargs >= 4) {
	CHECK_TYPE(XPATH_NUMBER);
	number = valuePop(ctxt);
	if (number != NULL)
	    num = (int) number->floatval;
    }
    if (nargs >= 3) {
	CHECK_TYPE(XPATH_NUMBER);
	position = valuePop(ctxt);
	if (position != NULL)
	    pos = (int) position->floatval;
    }
    CHECK_TYPE(XPATH_STRING);
    string = valuePop(ctxt);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_LOCATIONSET) &&
	 (ctxt->value->type != XPATH_NODESET)))
        XP_ERROR(XPATH_INVALID_TYPE)

    set = valuePop(ctxt);
    newset = xmlXPtrLocationSetCreate(NULL);
    if (set->nodesetval == NULL) {
        goto error;
    }
    if (set->type == XPATH_NODESET) {
	xmlXPathObjectPtr tmp;

	 
	tmp = xmlXPtrNewLocationSetNodeSet(set->nodesetval);
	xmlXPathFreeObject(set);
	set = tmp;
    }
    oldset = (xmlLocationSetPtr) set->user;

     
    for (i = 0;i < oldset->locNr;i++) {
#ifdef DEBUG_RANGES
	xmlXPathDebugDumpObject(stdout, oldset->locTab[i], 0);
#endif

	xmlXPtrGetStartPoint(oldset->locTab[i], &start, &startindex);
	xmlXPtrGetEndPoint(oldset->locTab[i], &end, &endindex);
	xmlXPtrAdvanceChar(&start, &startindex, 0);
	xmlXPtrGetLastChar(&end, &endindex);

#ifdef DEBUG_RANGES
	xmlGenericError(xmlGenericErrorContext,
		"from index %d of ->", startindex);
	xmlDebugDumpString(stdout, start->content);
	xmlGenericError(xmlGenericErrorContext, "\n");
	xmlGenericError(xmlGenericErrorContext,
		"to index %d of ->", endindex);
	xmlDebugDumpString(stdout, end->content);
	xmlGenericError(xmlGenericErrorContext, "\n");
#endif
	do {
            fend = end;
            fendindex = endindex;
	    found = xmlXPtrSearchString(string->stringval, &start, &startindex,
		                        &fend, &fendindex);
	    if (found == 1) {
		if (position == NULL) {
		    xmlXPtrLocationSetAdd(newset,
			 xmlXPtrNewRange(start, startindex, fend, fendindex));
		} else if (xmlXPtrAdvanceChar(&start, &startindex,
			                      pos - 1) == 0) {
		    if ((number != NULL) && (num > 0)) {
			int rindx;
			xmlNodePtr rend;
			rend = start;
			rindx = startindex - 1;
			if (xmlXPtrAdvanceChar(&rend, &rindx,
				               num) == 0) {
			    xmlXPtrLocationSetAdd(newset,
					xmlXPtrNewRange(start, startindex,
							rend, rindx));
			}
		    } else if ((number != NULL) && (num <= 0)) {
			xmlXPtrLocationSetAdd(newset,
				    xmlXPtrNewRange(start, startindex,
						    start, startindex));
		    } else {
			xmlXPtrLocationSetAdd(newset,
				    xmlXPtrNewRange(start, startindex,
						    fend, fendindex));
		    }
		}
		start = fend;
		startindex = fendindex;
		if (string->stringval[0] == 0)
		    startindex++;
	    }
	} while (found == 1);
    }

     
error:
    valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
    xmlXPathFreeObject(set);
    xmlXPathFreeObject(string);
    if (position) xmlXPathFreeObject(position);
    if (number) xmlXPathFreeObject(number);
}
