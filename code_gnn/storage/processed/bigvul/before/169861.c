xsltFreeExtElement(xsltExtElementPtr ext)
{
    if (ext == NULL)
        return;
    xmlFree(ext);
}
