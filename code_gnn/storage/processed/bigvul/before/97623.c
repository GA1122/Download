xmlXPathCompareNodeSetFloat(xmlXPathParserContextPtr ctxt, int inf, int strict,
	                    xmlXPathObjectPtr arg, xmlXPathObjectPtr f) {
    int i, ret = 0;
    xmlNodeSetPtr ns;
    xmlChar *str2;

    if ((f == NULL) || (arg == NULL) ||
	((arg->type != XPATH_NODESET) && (arg->type != XPATH_XSLT_TREE))) {
	xmlXPathReleaseObject(ctxt->context, arg);
	xmlXPathReleaseObject(ctxt->context, f);
        return(0);
    }
    ns = arg->nodesetval;
    if (ns != NULL) {
	for (i = 0;i < ns->nodeNr;i++) {
	     str2 = xmlXPathCastNodeToString(ns->nodeTab[i]);
	     if (str2 != NULL) {
		 valuePush(ctxt,
			   xmlXPathCacheNewString(ctxt->context, str2));
		 xmlFree(str2);
		 xmlXPathNumberFunction(ctxt, 1);
		 valuePush(ctxt, xmlXPathCacheObjectCopy(ctxt->context, f));
		 ret = xmlXPathCompareValues(ctxt, inf, strict);
		 if (ret)
		     break;
	     }
	}
    }
    xmlXPathReleaseObject(ctxt->context, arg);
    xmlXPathReleaseObject(ctxt->context, f);
    return(ret);
}
