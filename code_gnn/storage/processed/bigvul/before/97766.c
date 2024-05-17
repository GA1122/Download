xmlXPathPopNumber (xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr obj;
    double ret;

    obj = valuePop(ctxt);
    if (obj == NULL) {
	xmlXPathSetError(ctxt, XPATH_INVALID_OPERAND);
	return(0);
    }
    if (obj->type != XPATH_NUMBER)
	ret = xmlXPathCastToNumber(obj);
    else
        ret = obj->floatval;
    xmlXPathReleaseObject(ctxt->context, obj);
    return(ret);
}
