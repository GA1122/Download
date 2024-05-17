xsltDebugDumpExtensionsCallback(void *function ATTRIBUTE_UNUSED,
                                FILE * output, const xmlChar * name,
                                const xmlChar * URI,
                                const xmlChar * not_used ATTRIBUTE_UNUSED)
{
    if (!name || !URI)
        return;
    fprintf(output, "{%s}%s\n", URI, name);
}
