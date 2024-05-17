xsltFreeExtModule(xsltExtModulePtr ext)
{
    if (ext == NULL)
        return;
    xmlFree(ext);
}
