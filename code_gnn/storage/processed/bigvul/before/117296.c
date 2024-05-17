xmlXPtrBuildRangeNodeList(xmlXPathObjectPtr range) {
     
    xmlNodePtr list = NULL, last = NULL, parent = NULL, tmp;
     
    xmlNodePtr start, cur, end;
    int index1, index2;

    if (range == NULL)
	return(NULL);
    if (range->type != XPATH_RANGE)
	return(NULL);
    start = (xmlNodePtr) range->user;

    if (start == NULL)
	return(NULL);
    end = range->user2;
    if (end == NULL)
	return(xmlCopyNode(start, 1));

    cur = start;
    index1 = range->index;
    index2 = range->index2;
    while (cur != NULL) {
	if (cur == end) {
	    if (cur->type == XML_TEXT_NODE) {
		const xmlChar *content = cur->content;
		int len;

		if (content == NULL) {
		    tmp = xmlNewTextLen(NULL, 0);
		} else {
		    len = index2;
		    if ((cur == start) && (index1 > 1)) {
			content += (index1 - 1);
			len -= (index1 - 1);
			index1 = 0;
		    } else {
			len = index2;
		    }
		    tmp = xmlNewTextLen(content, len);
		}
		 
		if (list == NULL)
		    return(tmp);
		 
		if (last != NULL)
		    xmlAddNextSibling(last, tmp);
		else 
		    xmlAddChild(parent, tmp);
		return(list);
	    } else {
		tmp = xmlCopyNode(cur, 0);
		if (list == NULL)
		    list = tmp;
		else {
		    if (last != NULL)
			xmlAddNextSibling(last, tmp);
		    else
			xmlAddChild(parent, tmp);
		}
		last = NULL;
		parent = tmp;

		if (index2 > 1) {
		    end = xmlXPtrGetNthChild(cur, index2 - 1);
		    index2 = 0;
		}
		if ((cur == start) && (index1 > 1)) {
		    cur = xmlXPtrGetNthChild(cur, index1 - 1);
		    index1 = 0;
		} else {
		    cur = cur->children;
		}
		 
		continue;  
	    }
	} else if ((cur == start) &&
		   (list == NULL)   ) {
	    if ((cur->type == XML_TEXT_NODE) ||
		(cur->type == XML_CDATA_SECTION_NODE)) {
		const xmlChar *content = cur->content;

		if (content == NULL) {
		    tmp = xmlNewTextLen(NULL, 0);
		} else {
		    if (index1 > 1) {
			content += (index1 - 1);
		    }
		    tmp = xmlNewText(content);
		}
		last = list = tmp;
	    } else {
		if ((cur == start) && (index1 > 1)) {
		    tmp = xmlCopyNode(cur, 0);
		    list = tmp;
		    parent = tmp;
		    last = NULL;
		    cur = xmlXPtrGetNthChild(cur, index1 - 1);
		    index1 = 0;
		     
		    continue;  
		}
		tmp = xmlCopyNode(cur, 1);
		list = tmp;
		parent = NULL;
		last = tmp;
	    }
	} else {
	    tmp = NULL;
	    switch (cur->type) {
		case XML_DTD_NODE:
		case XML_ELEMENT_DECL:
		case XML_ATTRIBUTE_DECL:
		case XML_ENTITY_NODE:
		     
		    break;
		case XML_ENTITY_DECL:
		    TODO  
		    break;
		case XML_XINCLUDE_START:
		case XML_XINCLUDE_END:
		     
		    break;
		case XML_ATTRIBUTE_NODE:
		     
		    STRANGE
		    break;
		default:
		    tmp = xmlCopyNode(cur, 1);
		    break;
	    }
	    if (tmp != NULL) {
		if ((list == NULL) || ((last == NULL) && (parent == NULL)))  {
		    STRANGE
		    return(NULL);
		}
		if (last != NULL)
		    xmlAddNextSibling(last, tmp);
		else {
		    xmlAddChild(parent, tmp);
		    last = tmp;
		}
	    }
	}
	 
	if ((list == NULL) || ((last == NULL) && (parent == NULL)))  {
	    STRANGE
	    return(NULL);
	}
	cur = xmlXPtrAdvanceNode(cur, NULL);
    }
    return(list);
}
