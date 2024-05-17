exsltFuncInit (xsltTransformContextPtr ctxt, const xmlChar *URI) {
    exsltFuncData *ret;
    xsltStylesheetPtr tmp;
    exsltFuncImportRegData ch;
    xmlHashTablePtr hash;

    ret = (exsltFuncData *) xmlMalloc (sizeof(exsltFuncData));
    if (ret == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncInit: not enough memory\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(exsltFuncData));

    ret->result = NULL;
    ret->error = 0;

    ch.hash = (xmlHashTablePtr) xsltStyleGetExtData(ctxt->style, URI);
    ret->funcs = ch.hash;
    xmlHashScanFull(ch.hash, (xmlHashScannerFull) exsltFuncRegisterFunc, ctxt);
    tmp = ctxt->style;
    ch.ctxt = ctxt;
    while ((tmp=xsltNextImport(tmp))!=NULL) {
	hash = xsltGetExtInfo(tmp, URI);
	if (hash != NULL) {
	    xmlHashScanFull(hash,
		    (xmlHashScannerFull) exsltFuncRegisterImportFunc, &ch);
	}
    }

    return(ret);
}
