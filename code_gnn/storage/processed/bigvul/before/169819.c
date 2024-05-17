exsltStrConcatFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr obj;
    xmlChar *ret = NULL;
    int i;

    if (nargs  != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (!xmlXPathStackIsNodeSet(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    obj = valuePop (ctxt);

    if (xmlXPathNodeSetIsEmpty(obj->nodesetval)) {
	xmlXPathReturnEmptyString(ctxt);
	return;
    }

    for (i = 0; i < obj->nodesetval->nodeNr; i++) {
	xmlChar *tmp;
	tmp = xmlXPathCastNodeToString(obj->nodesetval->nodeTab[i]);

	ret = xmlStrcat (ret, tmp);

	xmlFree(tmp);
    }

    xmlXPathFreeObject (obj);

    xmlXPathReturnString(ctxt, ret);
}
