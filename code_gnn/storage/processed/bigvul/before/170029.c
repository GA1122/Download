xsltFreeRVTs(xsltTransformContextPtr ctxt)
{
    xmlDocPtr cur, next;

    if (ctxt == NULL)
	return;
     
    cur = ctxt->localRVT;
    while (cur != NULL) {
        next = (xmlDocPtr) cur->next;
	if (cur->_private != NULL) {
	    xsltFreeDocumentKeys(cur->_private);
	    xmlFree(cur->_private);
	}
	xmlFreeDoc(cur);
	cur = next;
    }
    ctxt->localRVT = NULL;
     
    cur = ctxt->tmpRVT;
    while (cur != NULL) {
        next = (xmlDocPtr) cur->next;
	if (cur->_private != NULL) {
	    xsltFreeDocumentKeys(cur->_private);
	    xmlFree(cur->_private);
	}
	xmlFreeDoc(cur);
	cur = next;
    }
    ctxt->tmpRVT = NULL;
     
    cur = ctxt->persistRVT;
    while (cur != NULL) {
        next = (xmlDocPtr) cur->next;
	if (cur->_private != NULL) {
	    xsltFreeDocumentKeys(cur->_private);
	    xmlFree(cur->_private);
	}
	xmlFreeDoc(cur);
	cur = next;
    }
    ctxt->persistRVT = NULL;
}
