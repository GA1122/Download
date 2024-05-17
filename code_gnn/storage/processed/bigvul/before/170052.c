xsltCleanupStylesheetTree(xmlDocPtr doc ATTRIBUTE_UNUSED,
			  xmlNodePtr rootElem ATTRIBUTE_UNUSED)
{
#if 0  
    xmlNodePtr cur;

    if ((doc == NULL) || (rootElem == NULL) ||
	(rootElem->type != XML_ELEMENT_NODE) ||
	(doc != rootElem->doc))
	return(-1);

     

    cur = rootElem;
    while (cur != NULL) {
	if (cur->type == XML_ELEMENT_NODE) {
	     
	    cur->psvi = NULL;
	    if (cur->children) {
		cur = cur->children;
		continue;
	    }
	}

leave_node:
	if (cur == rootElem)
	    break;
	if (cur->next != NULL)
	    cur = cur->next;
	else {
	    cur = cur->parent;
	    if (cur == NULL)
		break;
	    goto leave_node;
	}
    }
#endif  
    return(0);
}
