xsltForwardsCompatUnkownItemCreate(xsltCompilerCtxtPtr cctxt)
{
    xsltStyleItemUknownPtr item;

    item = (xsltStyleItemUknownPtr) xmlMalloc(sizeof(xsltStyleItemUknown));
    if (item == NULL) {
	xsltTransformError(NULL, cctxt->style, NULL,
	    "Internal error in xsltForwardsCompatUnkownItemCreate(): "
	    "Failed to allocate memory.\n");
	cctxt->style->errors++;
	return(NULL);
    }
    memset(item, 0, sizeof(xsltStyleItemUknown));
    item->type = XSLT_FUNC_UNKOWN_FORWARDS_COMPAT;
     
    item->next = cctxt->style->preComps;
    cctxt->style->preComps = (xsltElemPreCompPtr) item;
    return(item);
}
