xmlXPathPErrMemory(xmlXPathParserContextPtr ctxt, const char *extra)
{
    if (ctxt == NULL)
	xmlXPathErrMemory(NULL, extra);
    else {
	ctxt->error = XPATH_MEMORY_ERROR;
	xmlXPathErrMemory(ctxt->context, extra);
    }
}
