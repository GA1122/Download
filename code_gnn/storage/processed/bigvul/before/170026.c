xsltExtensionInstructionResultFinalize(xsltTransformContextPtr ctxt)
{
    xmlDocPtr cur;

    if (ctxt == NULL)
	return(-1);
    if (ctxt->localRVTBase == NULL)
	return(0);
     
    cur = ctxt->localRVTBase;
    do {
	cur->psvi = NULL;
	cur = (xmlDocPtr) cur->next;
    } while (cur != NULL);
    return(0);
}
