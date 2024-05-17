xmlXPathFreeObject(xmlXPathObjectPtr obj) {
    if (obj == NULL) return;
    if ((obj->type == XPATH_NODESET) || (obj->type == XPATH_XSLT_TREE)) {
	if (obj->boolval) {
#if 0
	    if (obj->user != NULL) {
                xmlXPathFreeNodeSet(obj->nodesetval);
		xmlFreeNodeList((xmlNodePtr) obj->user);
	    } else
#endif
	    obj->type = XPATH_XSLT_TREE;  
	    if (obj->nodesetval != NULL)
		xmlXPathFreeValueTree(obj->nodesetval);
	} else {
	    if (obj->nodesetval != NULL)
		xmlXPathFreeNodeSet(obj->nodesetval);
	}
#ifdef LIBXML_XPTR_ENABLED
    } else if (obj->type == XPATH_LOCATIONSET) {
	if (obj->user != NULL)
	    xmlXPtrFreeLocationSet(obj->user);
#endif
    } else if (obj->type == XPATH_STRING) {
	if (obj->stringval != NULL)
	    xmlFree(obj->stringval);
    }
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageReleased(NULL, obj->type);
#endif
    xmlFree(obj);
}
