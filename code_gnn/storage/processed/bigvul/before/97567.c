xmlXPathCacheConvertNumber(xmlXPathContextPtr ctxt, xmlXPathObjectPtr val) {
    xmlXPathObjectPtr ret;

    if (val == NULL)
	return(xmlXPathCacheNewFloat(ctxt, 0.0));
    if (val->type == XPATH_NUMBER)
	return(val);
    ret = xmlXPathCacheNewFloat(ctxt, xmlXPathCastToNumber(val));
    xmlXPathReleaseObject(ctxt, val);
    return(ret);
}
