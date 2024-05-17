xmlXPtrBuildNodeList(xmlXPathObjectPtr obj) {
    xmlNodePtr list = NULL, last = NULL;
    int i;

    if (obj == NULL)
	return(NULL);
    switch (obj->type) {
        case XPATH_NODESET: {
	    xmlNodeSetPtr set = obj->nodesetval;
	    if (set == NULL)
		return(NULL);
	    for (i = 0;i < set->nodeNr;i++) {
		if (set->nodeTab[i] == NULL)
		    continue;
		switch (set->nodeTab[i]->type) {
		    case XML_TEXT_NODE:
		    case XML_CDATA_SECTION_NODE:
		    case XML_ELEMENT_NODE:
		    case XML_ENTITY_REF_NODE:
		    case XML_ENTITY_NODE:
		    case XML_PI_NODE:
		    case XML_COMMENT_NODE:
		    case XML_DOCUMENT_NODE:
		    case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
		    case XML_DOCB_DOCUMENT_NODE:
#endif
		    case XML_XINCLUDE_START:
		    case XML_XINCLUDE_END:
			break;
		    case XML_ATTRIBUTE_NODE:
		    case XML_NAMESPACE_DECL:
		    case XML_DOCUMENT_TYPE_NODE:
		    case XML_DOCUMENT_FRAG_NODE:
		    case XML_NOTATION_NODE:
		    case XML_DTD_NODE:
		    case XML_ELEMENT_DECL:
		    case XML_ATTRIBUTE_DECL:
		    case XML_ENTITY_DECL:
			continue;  
		}
		if (last == NULL)
		    list = last = xmlCopyNode(set->nodeTab[i], 1);
		else {
		    xmlAddNextSibling(last, xmlCopyNode(set->nodeTab[i], 1));
		    if (last->next != NULL)
			last = last->next;
		}
	    }
	    break;
	}
	case XPATH_LOCATIONSET: {
	    xmlLocationSetPtr set = (xmlLocationSetPtr) obj->user;
	    if (set == NULL)
		return(NULL);
	    for (i = 0;i < set->locNr;i++) {
		if (last == NULL)
		    list = last = xmlXPtrBuildNodeList(set->locTab[i]);
		else
		    xmlAddNextSibling(last,
			    xmlXPtrBuildNodeList(set->locTab[i]));
		if (last != NULL) {
		    while (last->next != NULL)
			last = last->next;
		}
	    }
	    break;
	}
	case XPATH_RANGE:
	    return(xmlXPtrBuildRangeNodeList(obj));
	case XPATH_POINT:
	    return(xmlCopyNode(obj->user, 0));
	default:
	    break;
    }
    return(list);
}
