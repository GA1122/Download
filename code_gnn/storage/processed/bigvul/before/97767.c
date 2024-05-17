xmlXPathPopString (xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr obj;
    xmlChar * ret;

    obj = valuePop(ctxt);
    if (obj == NULL) {
	xmlXPathSetError(ctxt, XPATH_INVALID_OPERAND);
	return(NULL);
    }
    ret = xmlXPathCastToString(obj);	 
     
    if (obj->stringval == ret)
	obj->stringval = NULL;
    xmlXPathReleaseObject(ctxt->context, obj);
    return(ret);
}
