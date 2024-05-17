xsltCountKeys(xsltTransformContextPtr ctxt)
{
    xsltStylesheetPtr style;
    xsltKeyDefPtr keyd;

    if (ctxt == NULL)
	return(-1);

     
    ctxt->hasTemplKeyPatterns = 0;
    style = ctxt->style;
    while (style != NULL) {
	if (style->keyMatch != NULL) {
	    ctxt->hasTemplKeyPatterns = 1;
	    break;
	}
	style = xsltNextImport(style);
    }
     
    ctxt->nbKeys = 0;
    style = ctxt->style;
    while (style != NULL) {
	keyd = style->keys;
	while (keyd) {
	    ctxt->nbKeys++;
	    keyd = keyd->next;
	}
	style = xsltNextImport(style);
    }
    return(ctxt->nbKeys);
}
