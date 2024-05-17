xsltCompilerBuildInScopeNsList(xsltCompilerCtxtPtr cctxt, xmlNodePtr node)
{
    xsltNsListContainerPtr nsi = NULL;
    xmlNsPtr *list = NULL, ns;
    int i, maxns = 5;
     
    while (node != NULL) {
        if (node->type == XML_ELEMENT_NODE) {
            ns = node->nsDef;
            while (ns != NULL) {
                if (nsi == NULL) {
		    nsi = (xsltNsListContainerPtr)
			xmlMalloc(sizeof(xsltNsListContainer));
		    if (nsi == NULL) {
			xsltTransformError(NULL, cctxt->style, NULL,
			    "xsltCompilerBuildInScopeNsList: "
			    "malloc failed!\n");
			goto internal_err;
		    }
		    memset(nsi, 0, sizeof(xsltNsListContainer));
                    nsi->list =
                        (xmlNsPtr *) xmlMalloc(maxns * sizeof(xmlNsPtr));
                    if (nsi->list == NULL) {
			xsltTransformError(NULL, cctxt->style, NULL,
			    "xsltCompilerBuildInScopeNsList: "
			    "malloc failed!\n");
			goto internal_err;
                    }
                    nsi->list[0] = NULL;
                }
		 
                for (i = 0; i < nsi->totalNumber; i++) {
                    if ((ns->prefix == nsi->list[i]->prefix) ||
                        (xmlStrEqual(ns->prefix, nsi->list[i]->prefix)))
		    break;
                }
                if (i >= nsi->totalNumber) {
                    if (nsi->totalNumber +1 >= maxns) {
                        maxns *= 2;
			nsi->list =
			    (xmlNsPtr *) xmlRealloc(nsi->list,
				maxns * sizeof(xmlNsPtr));
                        if (nsi->list == NULL) {
                            xsltTransformError(NULL, cctxt->style, NULL,
				"xsltCompilerBuildInScopeNsList: "
				"realloc failed!\n");
				goto internal_err;
                        }
                    }
                    nsi->list[nsi->totalNumber++] = ns;
                    nsi->list[nsi->totalNumber] = NULL;
                }

                ns = ns->next;
            }
        }
        node = node->parent;
    }
    if (nsi == NULL)
	return(NULL);
     
    nsi->xpathNumber = nsi->totalNumber;
    for (i = 0; i < nsi->totalNumber; i++) {
	if (nsi->list[i]->prefix == NULL) {
	    ns = nsi->list[i];
	    nsi->list[i] = nsi->list[nsi->totalNumber-1];
	    nsi->list[nsi->totalNumber-1] = ns;
	    nsi->xpathNumber--;
	    break;
	}
    }
     
    if (xsltPointerListAddSize(
	(xsltPointerListPtr)cctxt->psData->inScopeNamespaces,
	(void *) nsi, 5) == -1)
    {
	xmlFree(nsi);
	nsi = NULL;
	xsltTransformError(NULL, cctxt->style, NULL,
	    "xsltCompilerBuildInScopeNsList: failed to add ns-info.\n");
	goto internal_err;
    }
     
    if (cctxt->inode != NULL)
	cctxt->inode->nsChanged = 1;

    return(nsi);

internal_err:
    if (list != NULL)
	xmlFree(list);
    cctxt->style->errors++;
    return(NULL);
}