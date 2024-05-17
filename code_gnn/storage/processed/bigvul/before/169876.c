xsltPreComputeExtModuleElement(xsltStylesheetPtr style, xmlNodePtr inst)
{
    xsltExtElementPtr ext;
    xsltElemPreCompPtr comp = NULL;

    if ((style == NULL) || (inst == NULL) ||
        (inst->type != XML_ELEMENT_NODE) || (inst->ns == NULL))
        return (NULL);

    xmlMutexLock(xsltExtMutex);

    ext = (xsltExtElementPtr)
        xmlHashLookup2(xsltElementsHash, inst->name, inst->ns->href);

    xmlMutexUnlock(xsltExtMutex);

     
    if (ext == NULL)
        return (NULL);

    if (ext->precomp != NULL) {
	 
        comp = ext->precomp(style, inst, ext->transform);
    }
    if (comp == NULL) {
	 
        comp = xsltNewElemPreComp(style, inst, ext->transform);
    }

    return (comp);
}
