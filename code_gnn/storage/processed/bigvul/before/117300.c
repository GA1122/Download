xmlXPtrErr(xmlXPathParserContextPtr ctxt, int error,
           const char * msg, const xmlChar *extra)
{
    if (ctxt != NULL)
        ctxt->error = error;
    if ((ctxt == NULL) || (ctxt->context == NULL)) {
	__xmlRaiseError(NULL, NULL, NULL,
			NULL, NULL, XML_FROM_XPOINTER, error,
			XML_ERR_ERROR, NULL, 0,
			(const char *) extra, NULL, NULL, 0, 0,
			msg, extra);
	return;
    }
    ctxt->context->lastError.domain = XML_FROM_XPOINTER;
    ctxt->context->lastError.code = error;
    ctxt->context->lastError.level = XML_ERR_ERROR;
    ctxt->context->lastError.str1 = (char *) xmlStrdup(ctxt->base);
    ctxt->context->lastError.int1 = ctxt->cur - ctxt->base;
    ctxt->context->lastError.node = ctxt->context->debugNode;
    if (ctxt->context->error != NULL) {
	ctxt->context->error(ctxt->context->userData,
	                     &ctxt->context->lastError);
    } else {
	__xmlRaiseError(NULL, NULL, NULL,
			NULL, ctxt->context->debugNode, XML_FROM_XPOINTER,
			error, XML_ERR_ERROR, NULL, 0,
			(const char *) extra, (const char *) ctxt->base, NULL,
			ctxt->cur - ctxt->base, 0,
			msg, extra);
    }
}
