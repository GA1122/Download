xmlXPathCacheConvertString(xmlXPathContextPtr ctxt, xmlXPathObjectPtr val) {
    xmlChar *res = NULL;

    if (val == NULL)
	return(xmlXPathCacheNewCString(ctxt, ""));

    switch (val->type) {
    case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
	xmlGenericError(xmlGenericErrorContext, "STRING: undefined\n");
#endif
	break;
    case XPATH_NODESET:
    case XPATH_XSLT_TREE:
	res = xmlXPathCastNodeSetToString(val->nodesetval);
	break;
    case XPATH_STRING:
	return(val);
    case XPATH_BOOLEAN:
	res = xmlXPathCastBooleanToString(val->boolval);
	break;
    case XPATH_NUMBER:
	res = xmlXPathCastNumberToString(val->floatval);
	break;
    case XPATH_USERS:
    case XPATH_POINT:
    case XPATH_RANGE:
    case XPATH_LOCATIONSET:
	TODO;
	break;
    }
    xmlXPathReleaseObject(ctxt, val);
    if (res == NULL)
	return(xmlXPathCacheNewCString(ctxt, ""));
    return(xmlXPathCacheWrapString(ctxt, res));
}
