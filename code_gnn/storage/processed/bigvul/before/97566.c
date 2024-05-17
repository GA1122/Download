xmlXPathCacheConvertBoolean(xmlXPathContextPtr ctxt, xmlXPathObjectPtr val) {
    xmlXPathObjectPtr ret;

    if (val == NULL)
	return(xmlXPathCacheNewBoolean(ctxt, 0));
    if (val->type == XPATH_BOOLEAN)
	return(val);
    ret = xmlXPathCacheNewBoolean(ctxt, xmlXPathCastToBoolean(val));
    xmlXPathReleaseObject(ctxt, val);
    return(ret);
}
