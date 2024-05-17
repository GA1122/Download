xsltGetExtInfo(xsltStylesheetPtr style, const xmlChar * URI)
{
    xsltExtDataPtr data;

     

    if (style != NULL && style->extInfos != NULL) {
        data = xmlHashLookup(style->extInfos, URI);
        if (data != NULL && data->extData != NULL)
            return data->extData;
    }
    return NULL;
}
