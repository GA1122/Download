xsltApplyStripSpaces(xsltTransformContextPtr ctxt, xmlNodePtr node) {
    xmlNodePtr current;
#ifdef WITH_XSLT_DEBUG_PROCESS
    int nb = 0;
#endif


    current = node;
    while (current != NULL) {
	 
	if ((IS_XSLT_REAL_NODE(current)) &&
	    (current->children != NULL) &&
	    (xsltFindElemSpaceHandling(ctxt, current))) {
	    xmlNodePtr delete = NULL, cur = current->children;

	    while (cur != NULL) {
		if (IS_BLANK_NODE(cur))
		    delete = cur;

		cur = cur->next;
		if (delete != NULL) {
		    xmlUnlinkNode(delete);
		    xmlFreeNode(delete);
		    delete = NULL;
#ifdef WITH_XSLT_DEBUG_PROCESS
		    nb++;
#endif
		}
	    }
	}

	 
	if (node->type == XML_ENTITY_REF_NODE) {
	     
	    xsltApplyStripSpaces(ctxt, node->children);
	}
	if ((current->children != NULL) &&
            (current->type != XML_ENTITY_REF_NODE)) {
	    current = current->children;
	} else if (current->next != NULL) {
	    current = current->next;
	} else {
	    do {
		current = current->parent;
		if (current == NULL)
		    break;
		if (current == node)
		    goto done;
		if (current->next != NULL) {
		    current = current->next;
		    break;
		}
	    } while (current != NULL);
	}
    }

done:
#ifdef WITH_XSLT_DEBUG_PROCESS
    XSLT_TRACE(ctxt,XSLT_TRACE_STRIP_SPACES,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltApplyStripSpaces: removed %d ignorable blank node\n", nb));
#endif
    return;
}
