exsltFuncShutdown (xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED,
		   const xmlChar *URI ATTRIBUTE_UNUSED,
		   exsltFuncData *data) {
    if (data->result != NULL)
	xmlXPathFreeObject(data->result);
    xmlFree(data);
}
