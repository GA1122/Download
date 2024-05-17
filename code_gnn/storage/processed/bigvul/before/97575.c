xmlXPathCacheObjectCopy(xmlXPathContextPtr ctxt, xmlXPathObjectPtr val)
{
    if (val == NULL)
	return(NULL);

    if (XP_HAS_CACHE(ctxt)) {
	switch (val->type) {
	    case XPATH_NODESET:
		return(xmlXPathCacheWrapNodeSet(ctxt,
		    xmlXPathNodeSetMerge(NULL, val->nodesetval)));
	    case XPATH_STRING:
		return(xmlXPathCacheNewString(ctxt, val->stringval));
	    case XPATH_BOOLEAN:
		return(xmlXPathCacheNewBoolean(ctxt, val->boolval));
	    case XPATH_NUMBER:
		return(xmlXPathCacheNewFloat(ctxt, val->floatval));
	    default:
		break;
	}
    }
    return(xmlXPathObjectCopy(val));
}
