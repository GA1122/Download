xsltMergeAttrElemList(xsltStylesheetPtr style,
		      xsltAttrElemPtr list, xsltAttrElemPtr old) {
    xsltAttrElemPtr cur;
    int add;

    while (old != NULL) {
	if ((old->attr == NULL) && (old->set == NULL)) {
	    old = old->next;
	    continue;
	}
	 
	cur = list;
	add = 1;
	while (cur != NULL) {
	    if ((cur->attr == NULL) && (cur->set == NULL)) {
		if (cur->next == NULL)
		    break;
		cur = cur->next;
		continue;
	    }
	    if ((cur->set != NULL) && (cur->set == old->set)) {
		add = 0;
		break;
	    }
	    if (cur->set != NULL) {
		if (cur->next == NULL)
		    break;
		cur = cur->next;
		continue;
	    }
	    if (old->set != NULL) {
		if (cur->next == NULL)
		    break;
		cur = cur->next;
		continue;
	    }
	    if (cur->attr == old->attr) {
		xsltGenericError(xsltGenericErrorContext,
	     "xsl:attribute-set : use-attribute-sets recursion detected\n");
		return(list);
	    }
	    if (cur->next == NULL)
		break;
            cur = cur->next;
	}

	if (add == 1) {
	     
	    if (cur == NULL) {
		list = xsltNewAttrElem(old->attr);
		if (old->set != NULL) {
		    list->set = xmlDictLookup(style->dict, old->set, -1);
		    if (old->ns != NULL)
			list->ns = xmlDictLookup(style->dict, old->ns, -1);
		}
	    } else if (add) {
		cur->next = xsltNewAttrElem(old->attr);
		if (old->set != NULL) {
		    cur->next->set = xmlDictLookup(style->dict, old->set, -1);
		    if (old->ns != NULL)
			cur->next->ns = xmlDictLookup(style->dict, old->ns, -1);
		}
	    }
	}

	old = old->next;
    }
    return(list);
}
