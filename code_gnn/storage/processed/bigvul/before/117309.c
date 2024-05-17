xmlXPtrGetChildNo(xmlXPathParserContextPtr ctxt, int indx) {
    xmlNodePtr cur = NULL;
    xmlXPathObjectPtr obj;
    xmlNodeSetPtr oldset;

    CHECK_TYPE(XPATH_NODESET);
    obj = valuePop(ctxt);
    oldset = obj->nodesetval;
    if ((indx <= 0) || (oldset == NULL) || (oldset->nodeNr != 1)) {
	xmlXPathFreeObject(obj);
	valuePush(ctxt, xmlXPathNewNodeSet(NULL));
	return;
    }
    cur = xmlXPtrGetNthChild(oldset->nodeTab[0], indx);
    if (cur == NULL) {
	xmlXPathFreeObject(obj);
	valuePush(ctxt, xmlXPathNewNodeSet(NULL));
	return;
    }
    oldset->nodeTab[0] = cur;
    valuePush(ctxt, obj);
}
