xmlXPathPopNodeSet (xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr obj;
    xmlNodeSetPtr ret;

    if (ctxt == NULL) return(NULL);
    if (ctxt->value == NULL) {
	xmlXPathSetError(ctxt, XPATH_INVALID_OPERAND);
	return(NULL);
    }
    if (!xmlXPathStackIsNodeSet(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return(NULL);
    }
    obj = valuePop(ctxt);
    ret = obj->nodesetval;
#if 0
     
    if (obj->boolval && obj->user != NULL)
        xmlFreeNodeList((xmlNodePtr) obj->user);
#endif
    obj->nodesetval = NULL;
    xmlXPathReleaseObject(ctxt->context, obj);
    return(ret);
}
