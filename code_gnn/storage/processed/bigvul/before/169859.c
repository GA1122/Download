xsltFreeExtDef(xsltExtDefPtr extensiond)
{
    if (extensiond == NULL)
        return;
    if (extensiond->prefix != NULL)
        xmlFree(extensiond->prefix);
    if (extensiond->URI != NULL)
        xmlFree(extensiond->URI);
    xmlFree(extensiond);
}
