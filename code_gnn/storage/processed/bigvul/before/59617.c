rngTest(const char *filename,
            const char *resul ATTRIBUTE_UNUSED,
            const char *errr ATTRIBUTE_UNUSED,
            int options) {
    const char *base = baseFilename(filename);
    const char *base2;
    const char *instance;
    xmlRelaxNGParserCtxtPtr ctxt;
    xmlRelaxNGPtr schemas;
    int res = 0, len, ret = 0;
    char pattern[500];
    char prefix[500];
    char result[500];
    char err[500];
    glob_t globbuf;
    size_t i;
    char count = 0;

     
    ctxt = xmlRelaxNGNewParserCtxt(filename);
    xmlRelaxNGSetParserErrors(ctxt,
         (xmlRelaxNGValidityErrorFunc) testErrorHandler,
         (xmlRelaxNGValidityWarningFunc) testErrorHandler,
	 ctxt);
    schemas = xmlRelaxNGParse(ctxt);
    xmlRelaxNGFreeParserCtxt(ctxt);

     
    len = strlen(base);
    if ((len > 499) || (len < 5)) {
        xmlRelaxNGFree(schemas);
	return(-1);
    }
    len -= 4;  
    memcpy(prefix, base, len);
    prefix[len] = 0;

    snprintf(pattern, 499, "./test/relaxng/%s_?.xml", prefix);
    pattern[499] = 0;

    globbuf.gl_offs = 0;
    glob(pattern, GLOB_DOOFFS, NULL, &globbuf);
    for (i = 0;i < globbuf.gl_pathc;i++) {
        testErrorsSize = 0;
	testErrors[0] = 0;
        instance = globbuf.gl_pathv[i];
	base2 = baseFilename(instance);
	len = strlen(base2);
	if ((len > 6) && (base2[len - 6] == '_')) {
	    count = base2[len - 5];
	    snprintf(result, 499, "result/relaxng/%s_%c",
		     prefix, count);
	    result[499] = 0;
	    snprintf(err, 499, "result/relaxng/%s_%c.err",
		     prefix, count);
	    err[499] = 0;
	} else {
	    fprintf(stderr, "don't know how to process %s\n", instance);
	    continue;
	}
	if (schemas == NULL) {
	} else {
	    nb_tests++;
	    ret = rngOneTest(filename, instance, result, err,
	                         options, schemas);
	    if (res != 0)
		ret = res;
	}
    }
    globfree(&globbuf);
    xmlRelaxNGFree(schemas);

    return(ret);
}