xsltExtModuleRegisterDynamic(const xmlChar * URI)
{

    xmlModulePtr m;
    exsltRegisterFunction regfunc;
    xmlChar *ext_name;
    char module_filename[PATH_MAX];
    const xmlChar *ext_directory = NULL;
    const xmlChar *protocol = NULL;
    xmlChar *i, *regfunc_name;
    void *vregfunc;
    int rc;

     
    if (URI == NULL)
        return (-1);

    if (NULL == xsltModuleHash) {
        xsltModuleHash = xmlHashCreate(5);
        if (xsltModuleHash == NULL)
            return (-1);
    }

    xmlMutexLock(xsltExtMutex);

     
    if (xmlHashLookup(xsltModuleHash, URI) != NULL) {
        xmlMutexUnlock(xsltExtMutex);
        return (-1);
    }
    xmlMutexUnlock(xsltExtMutex);

     
    protocol = xmlStrstr(URI, BAD_CAST "://");
    if (protocol == NULL) {
        ext_name = xmlStrdup(URI);
    } else {
        ext_name = xmlStrdup(protocol + 3);
    }
    if (ext_name == NULL) {
        return (-1);
    }

    i = ext_name;
    while ('\0' != *i) {
        if (('/' == *i) || ('\\' == *i) || ('.' == *i) || ('-' == *i))
            *i = '_';
        i++;
    }

    if (*(i - 1) == '_')
        *i = '\0';

     
    ext_directory = (xmlChar *) getenv("LIBXSLT_PLUGINS_PATH");

    if (NULL == ext_directory) {
        ext_directory = BAD_CAST LIBXSLT_DEFAULT_PLUGINS_PATH();
	if (NULL == ext_directory)
	  return (-1);
    }
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    else
      xsltGenericDebug(xsltGenericDebugContext,
		       "LIBXSLT_PLUGINS_PATH is %s\n", ext_directory);
#endif

     
    xmlStrPrintf((xmlChar *) module_filename, sizeof(module_filename),
                 BAD_CAST "%s/%s%s",
                 ext_directory, ext_name, LIBXML_MODULE_EXTENSION);

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext,
                     "Attempting to load plugin: %s for URI: %s\n",
                     module_filename, URI);
#endif

    if (1 != xmlCheckFilename(module_filename)) {

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
                     "xmlCheckFilename failed for plugin: %s\n", module_filename);
#endif

        xmlFree(ext_name);
        return (-1);
    }

     
    m = xmlModuleOpen(module_filename, 0);
    if (NULL == m) {

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
                     "xmlModuleOpen failed for plugin: %s\n", module_filename);
#endif

        xmlFree(ext_name);
        return (-1);
    }

     
    regfunc_name = xmlStrdup(ext_name);
    regfunc_name = xmlStrcat(regfunc_name, BAD_CAST "_init");

    vregfunc = NULL;
    rc = xmlModuleSymbol(m, (const char *) regfunc_name, &vregfunc);
    regfunc = vregfunc;
    if (0 == rc) {
         
        (*regfunc) ();

         
        xmlMutexLock(xsltExtMutex);
        xmlHashAddEntry(xsltModuleHash, URI, (void *) m);
        xmlMutexUnlock(xsltExtMutex);
    } else {

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
                     "xmlModuleSymbol failed for plugin: %s, regfunc: %s\n",
                     module_filename, regfunc_name);
#endif

         
        xmlModuleClose(m);
    }

    xmlFree(ext_name);
    xmlFree(regfunc_name);
    return (NULL == regfunc) ? -1 : 0;
}
