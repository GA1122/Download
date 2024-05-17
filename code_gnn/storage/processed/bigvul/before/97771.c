xmlXPathRegisterFuncLookup (xmlXPathContextPtr ctxt,
			    xmlXPathFuncLookupFunc f,
			    void *funcCtxt) {
    if (ctxt == NULL)
	return;
    ctxt->funcLookupFunc = f;
    ctxt->funcLookupData = funcCtxt;
}
