xsltEvalUserParams(xsltTransformContextPtr ctxt, const char **params) {
    int indx = 0;
    const xmlChar *name;
    const xmlChar *value;

    if (params == NULL)
	return(0);
    while (params[indx] != NULL) {
	name = (const xmlChar *) params[indx++];
	value = (const xmlChar *) params[indx++];
	if (xsltEvalOneUserParam(ctxt, name, value) != 0)
	    return(-1);
    }
    return 0;
}
