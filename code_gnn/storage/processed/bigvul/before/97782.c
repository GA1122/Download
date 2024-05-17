xmlXPathRoot(xmlXPathParserContextPtr ctxt) {
    if ((ctxt == NULL) || (ctxt->context == NULL))
	return;
    ctxt->context->node = (xmlNodePtr) ctxt->context->doc;
    valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
	ctxt->context->node));
}
