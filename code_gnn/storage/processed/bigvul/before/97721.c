xmlXPathNextChildElement(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL) {
	cur = ctxt->context->node;
	if (cur == NULL) return(NULL);
	 
	switch (cur->type) {
            case XML_ELEMENT_NODE:
	    case XML_DOCUMENT_FRAG_NODE:
	    case XML_ENTITY_REF_NODE:  
            case XML_ENTITY_NODE:
		cur = cur->children;
		if (cur != NULL) {
		    if (cur->type == XML_ELEMENT_NODE)
			return(cur);
		    do {
			cur = cur->next;
		    } while ((cur != NULL) &&
			(cur->type != XML_ELEMENT_NODE));
		    return(cur);
		}
		return(NULL);
            case XML_DOCUMENT_NODE:
            case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
	    case XML_DOCB_DOCUMENT_NODE:
#endif
		return(xmlDocGetRootElement((xmlDocPtr) cur));
	    default:
		return(NULL);
	}
	return(NULL);
    }
     
    switch (cur->type) {
	case XML_ELEMENT_NODE:
	case XML_TEXT_NODE:
	case XML_ENTITY_REF_NODE:
	case XML_ENTITY_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_PI_NODE:
	case XML_COMMENT_NODE:
	case XML_XINCLUDE_END:
	    break;
	   
	default:
	    return(NULL);
    }
    if (cur->next != NULL) {
	if (cur->next->type == XML_ELEMENT_NODE)
	    return(cur->next);
	cur = cur->next;
	do {
	    cur = cur->next;
	} while ((cur != NULL) && (cur->type != XML_ELEMENT_NODE));
	return(cur);
    }
    return(NULL);
}
