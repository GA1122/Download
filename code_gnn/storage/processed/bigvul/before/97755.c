xmlXPathObjectCopy(xmlXPathObjectPtr val) {
    xmlXPathObjectPtr ret;

    if (val == NULL)
	return(NULL);

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "copying object\n");
	return(NULL);
    }
    memcpy(ret, val , (size_t) sizeof(xmlXPathObject));
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, val->type);
#endif
    switch (val->type) {
	case XPATH_BOOLEAN:
	case XPATH_NUMBER:
	case XPATH_POINT:
	case XPATH_RANGE:
	    break;
	case XPATH_STRING:
	    ret->stringval = xmlStrdup(val->stringval);
	    break;
	case XPATH_XSLT_TREE:
#if 0
 
	    if ((val->nodesetval != NULL) &&
		(val->nodesetval->nodeTab != NULL)) {
		xmlNodePtr cur, tmp;
		xmlDocPtr top;

		ret->boolval = 1;
		top =  xmlNewDoc(NULL);
		top->name = (char *)
		    xmlStrdup(val->nodesetval->nodeTab[0]->name);
		ret->user = top;
		if (top != NULL) {
		    top->doc = top;
		    cur = val->nodesetval->nodeTab[0]->children;
		    while (cur != NULL) {
			tmp = xmlDocCopyNode(cur, top, 1);
			xmlAddChild((xmlNodePtr) top, tmp);
			cur = cur->next;
		    }
		}

		ret->nodesetval = xmlXPathNodeSetCreate((xmlNodePtr) top);
	    } else
		ret->nodesetval = xmlXPathNodeSetCreate(NULL);
	     
	    break;
#endif
	case XPATH_NODESET:
	    ret->nodesetval = xmlXPathNodeSetMerge(NULL, val->nodesetval);
	     
	    ret->boolval = 0;
	    break;
	case XPATH_LOCATIONSET:
#ifdef LIBXML_XPTR_ENABLED
	{
	    xmlLocationSetPtr loc = val->user;
	    ret->user = (void *) xmlXPtrLocationSetMerge(NULL, loc);
	    break;
	}
#endif
        case XPATH_USERS:
	    ret->user = val->user;
	    break;
        case XPATH_UNDEFINED:
	    xmlGenericError(xmlGenericErrorContext,
		    "xmlXPathObjectCopy: unsupported type %d\n",
		    val->type);
	    break;
    }
    return(ret);
}
