xsltNewElemPreComp(xsltStylesheetPtr style, xmlNodePtr inst,
                   xsltTransformFunction function)
{
    xsltElemPreCompPtr cur;

    cur = (xsltElemPreCompPtr) xmlMalloc(sizeof(xsltElemPreComp));
    if (cur == NULL) {
        xsltTransformError(NULL, style, NULL,
                           "xsltNewExtElement : malloc failed\n");
        return (NULL);
    }
    memset(cur, 0, sizeof(xsltElemPreComp));

    xsltInitElemPreComp(cur, style, inst, function,
                        (xsltElemPreCompDeallocator) xmlFree);

    return (cur);
}
