xmlXPtrInsideRange(xmlXPathParserContextPtr ctxt, xmlXPathObjectPtr loc) {
    if (loc == NULL)
	return(NULL);
    if ((ctxt == NULL) || (ctxt->context == NULL) ||
	(ctxt->context->doc == NULL))
	return(NULL);
    switch (loc->type) {
        case XPATH_POINT: {
	    xmlNodePtr node = (xmlNodePtr) loc->user;
	    switch (node->type) {
		case XML_PI_NODE:
		case XML_COMMENT_NODE:
		case XML_TEXT_NODE:
		case XML_CDATA_SECTION_NODE: {
		    if (node->content == NULL) {
			return(xmlXPtrNewRange(node, 0, node, 0));
		    } else {
			return(xmlXPtrNewRange(node, 0, node,
					       xmlStrlen(node->content)));
		    }
		}
		case XML_ATTRIBUTE_NODE:
		case XML_ELEMENT_NODE:
		case XML_ENTITY_REF_NODE:
		case XML_DOCUMENT_NODE:
		case XML_NOTATION_NODE:
		case XML_HTML_DOCUMENT_NODE: {
		    return(xmlXPtrNewRange(node, 0, node,
					   xmlXPtrGetArity(node)));
		}
		default:
		    break;
	    }
	    return(NULL);
	}
        case XPATH_RANGE: {
	    xmlNodePtr node = (xmlNodePtr) loc->user;
	    if (loc->user2 != NULL) {
		return(xmlXPtrNewRange(node, loc->index,
			               loc->user2, loc->index2));
	    } else {
		switch (node->type) {
		    case XML_PI_NODE:
		    case XML_COMMENT_NODE:
		    case XML_TEXT_NODE:
		    case XML_CDATA_SECTION_NODE: {
			if (node->content == NULL) {
			    return(xmlXPtrNewRange(node, 0, node, 0));
			} else {
			    return(xmlXPtrNewRange(node, 0, node,
						   xmlStrlen(node->content)));
			}
		    }
		    case XML_ATTRIBUTE_NODE:
		    case XML_ELEMENT_NODE:
		    case XML_ENTITY_REF_NODE:
		    case XML_DOCUMENT_NODE:
		    case XML_NOTATION_NODE:
		    case XML_HTML_DOCUMENT_NODE: {
			return(xmlXPtrNewRange(node, 0, node,
					       xmlXPtrGetArity(node)));
		    }
		    default:
			break;
		}
		return(NULL);
	    }
        }
	default:
	    TODO  
    }
    return(NULL);
}
