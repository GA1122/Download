xsltNewExtElement(xsltPreComputeFunction precomp,
                  xsltTransformFunction transform)
{
    xsltExtElementPtr cur;

    if (transform == NULL)
        return (NULL);

    cur = (xsltExtElementPtr) xmlMalloc(sizeof(xsltExtElement));
    if (cur == NULL) {
        xsltTransformError(NULL, NULL, NULL,
                           "xsltNewExtElement : malloc failed\n");
        return (NULL);
    }
    cur->precomp = precomp;
    cur->transform = transform;
    return (cur);
}
