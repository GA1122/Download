xmlXPathEqualNodeSetFloat(xmlXPathParserContextPtr ctxt,
    xmlXPathObjectPtr arg, double f, int neq) {
  int i, ret=0;
  xmlNodeSetPtr ns;
  xmlChar *str2;
  xmlXPathObjectPtr val;
  double v;

    if ((arg == NULL) ||
	((arg->type != XPATH_NODESET) && (arg->type != XPATH_XSLT_TREE)))
        return(0);

    ns = arg->nodesetval;
    if (ns != NULL) {
	for (i=0;i<ns->nodeNr;i++) {
	    str2 = xmlXPathCastNodeToString(ns->nodeTab[i]);
	    if (str2 != NULL) {
		valuePush(ctxt, xmlXPathCacheNewString(ctxt->context, str2));
		xmlFree(str2);
		xmlXPathNumberFunction(ctxt, 1);
		val = valuePop(ctxt);
		v = val->floatval;
		xmlXPathReleaseObject(ctxt->context, val);
		if (!xmlXPathIsNaN(v)) {
		    if ((!neq) && (v==f)) {
			ret = 1;
			break;
		    } else if ((neq) && (v!=f)) {
			ret = 1;
			break;
		    }
		} else {	 
		    if (neq)
			ret = 1;
		}
	    }
	}
    }

    return(ret);
}
