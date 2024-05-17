xmlXPtrRangeFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    int i;
    xmlXPathObjectPtr set;
    xmlLocationSetPtr oldset;
    xmlLocationSetPtr newset;

    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_LOCATIONSET) &&
	 (ctxt->value->type != XPATH_NODESET)))
        XP_ERROR(XPATH_INVALID_TYPE)

    set = valuePop(ctxt);
    if (set->type == XPATH_NODESET) {
	xmlXPathObjectPtr tmp;

	 
	tmp = xmlXPtrNewLocationSetNodeSet(set->nodesetval);
	xmlXPathFreeObject(set);
	set = tmp;
    }
    oldset = (xmlLocationSetPtr) set->user;

     
    newset = xmlXPtrLocationSetCreate(NULL);
    for (i = 0;i < oldset->locNr;i++) {
	xmlXPtrLocationSetAdd(newset,
		xmlXPtrCoveringRange(ctxt, oldset->locTab[i]));
    }

     
    valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
    xmlXPathFreeObject(set);
}
