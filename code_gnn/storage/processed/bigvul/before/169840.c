xsltDebugDumpExtModulesCallback(void *function ATTRIBUTE_UNUSED,
                                FILE * output, const xmlChar * URI,
                                const xmlChar * not_used ATTRIBUTE_UNUSED,
                                const xmlChar * not_used2 ATTRIBUTE_UNUSED)
{
    if (!URI)
        return;
    fprintf(output, "%s\n", URI);
}
