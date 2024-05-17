xsltFreeExtData(xsltExtDataPtr ext)
{
    if (ext == NULL)
        return;
    xmlFree(ext);
}
