xmlURIErrMemory(const char *extra)
{
 if (extra)
        __xmlRaiseError(NULL, NULL, NULL,
                        NULL, NULL, XML_FROM_URI,
                        XML_ERR_NO_MEMORY, XML_ERR_FATAL, NULL, 0,
                        extra, NULL, NULL, 0, 0,
 "Memory allocation failed : %s\n", extra);
 else
        __xmlRaiseError(NULL, NULL, NULL,
                        NULL, NULL, XML_FROM_URI,
                        XML_ERR_NO_MEMORY, XML_ERR_FATAL, NULL, 0,
                        NULL, NULL, NULL, 0, 0,
 "Memory allocation failed\n");
}
