xmlXPtrCoveringRange(xmlXPathParserContextPtr ctxt, xmlXPathObjectPtr loc) {
    if (loc == NULL)
	return(NULL);
    if ((ctxt == NULL) || (ctxt->context == NULL) ||
	(ctxt->context->doc == NULL))
	return(NULL);
    switch (loc->type) {
        case XPATH_POINT:
	    return(xmlXPtrNewRange(loc->user, loc->index,
			           loc->user, loc->index));
        case XPATH_RANGE:
	    if (loc->user2 != NULL) {
		return(xmlXPtrNewRange(loc->user, loc->index,
			              loc->user2, loc->index2));
	    } else {
		xmlNodePtr node = (xmlNodePtr) loc->user;
		if (node == (xmlNodePtr) ctxt->context->doc) {
		    return(xmlXPtrNewRange(node, 0, node,
					   xmlXPtrGetArity(node)));
		} else {
		    switch (node->type) {
			case XML_ATTRIBUTE_NODE:
			 
			    return(xmlXPtrNewRange(node, 0, node,
					           xmlXPtrGetArity(node)));
			case XML_ELEMENT_NODE:
			case XML_TEXT_NODE:
			case XML_CDATA_SECTION_NODE:
			case XML_ENTITY_REF_NODE:
			case XML_PI_NODE:
			case XML_COMMENT_NODE:
			case XML_DOCUMENT_NODE:
			case XML_NOTATION_NODE:
			case XML_HTML_DOCUMENT_NODE: {
			    int indx = xmlXPtrGetIndex(node);
			     
			    node = node->parent;
			    return(xmlXPtrNewRange(node, indx - 1,
					           node, indx + 1));
			}
			default:
			    return(NULL);
		    }
		}
	    }
	default:
	    TODO  
    }
    return(NULL);
}
