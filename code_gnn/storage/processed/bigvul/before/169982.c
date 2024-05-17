xsltApplyStylesheet(xsltStylesheetPtr style, xmlDocPtr doc,
                    const char **params)
{
    return (xsltApplyStylesheetInternal(style, doc, params, NULL, NULL, NULL));
}
