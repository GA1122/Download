xsltNewNsAlias(xsltCompilerCtxtPtr cctxt)
{
    xsltNsAliasPtr ret;

    if (cctxt == NULL)
	return(NULL);

    ret = (xsltNsAliasPtr) xmlMalloc(sizeof(xsltNsAlias));
    if (ret == NULL) {
	xsltTransformError(NULL, cctxt->style, NULL,
	    "Internal error in xsltNewNsAlias(): Memory allocation failed.\n");
	cctxt->style->errors++;
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltNsAlias));
     
    ret->next = cctxt->nsAliases;
    cctxt->nsAliases = ret;

    return(ret);
}
