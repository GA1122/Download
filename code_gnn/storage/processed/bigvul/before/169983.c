xsltApplyStylesheetUser(xsltStylesheetPtr style, xmlDocPtr doc,
                            const char **params, const char *output,
                            FILE * profile, xsltTransformContextPtr userCtxt)
{
    xmlDocPtr res;

    res = xsltApplyStylesheetInternal(style, doc, params, output,
	                              profile, userCtxt);
    return (res);
}
