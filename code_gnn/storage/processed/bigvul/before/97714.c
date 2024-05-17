xmlXPathNewParserContext(const xmlChar *str, xmlXPathContextPtr ctxt) {
    xmlXPathParserContextPtr ret;

    ret = (xmlXPathParserContextPtr) xmlMalloc(sizeof(xmlXPathParserContext));
    if (ret == NULL) {
        xmlXPathErrMemory(ctxt, "creating parser context\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathParserContext));
    ret->cur = ret->base = str;
    ret->context = ctxt;

    ret->comp = xmlXPathNewCompExpr();
    if (ret->comp == NULL) {
	xmlFree(ret->valueTab);
	xmlFree(ret);
	return(NULL);
    }
    if ((ctxt != NULL) && (ctxt->dict != NULL)) {
        ret->comp->dict = ctxt->dict;
	xmlDictReference(ret->comp->dict);
    }

    return(ret);
}
