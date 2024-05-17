xsltTransformCacheCreate(void)
{
    xsltTransformCachePtr ret;

    ret = (xsltTransformCachePtr) xmlMalloc(sizeof(xsltTransformCache));
    if (ret == NULL) {
	xsltTransformError(NULL, NULL, NULL,
	    "xsltTransformCacheCreate : malloc failed\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltTransformCache));
    return(ret);
}
