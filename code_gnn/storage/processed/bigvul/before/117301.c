xmlXPtrErrMemory(const char *extra)
{
    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_XPOINTER,
		    XML_ERR_NO_MEMORY, XML_ERR_ERROR, NULL, 0, extra,
		    NULL, NULL, 0, 0,
		    "Memory allocation failed : %s\n", extra);
}
