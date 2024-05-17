xmlXPathPopExternal (xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr obj;
    void * ret;

    if ((ctxt == NULL) || (ctxt->value == NULL)) {
	xmlXPathSetError(ctxt, XPATH_INVALID_OPERAND);
	return(NULL);
    }
    if (ctxt->value->type != XPATH_USERS) {
	xmlXPathSetTypeError(ctxt);
	return(NULL);
    }
    obj = valuePop(ctxt);
    ret = obj->user;
    obj->user = NULL;
    xmlXPathReleaseObject(ctxt->context, obj);
    return(ret);
}
