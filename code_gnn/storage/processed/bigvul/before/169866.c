xsltHashScannerModuleFree(void *payload ATTRIBUTE_UNUSED,
                          void *data ATTRIBUTE_UNUSED,
                          xmlChar * name ATTRIBUTE_UNUSED)
{
#ifdef WITH_MODULES
    xmlModuleClose(payload);
#endif
}
