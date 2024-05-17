rngStreamTest(const char *filename,
            const char *resul ATTRIBUTE_UNUSED,
            const char *errr ATTRIBUTE_UNUSED,
            int options) {
    const char *base = baseFilename(filename);
    const char *base2;
    const char *instance;
    int res = 0, len, ret;
    char pattern[500];
    char prefix[500];
    char result[500];
    char err[500];
    glob_t globbuf;
    size_t i;
    char count = 0;
    xmlTextReaderPtr reader;
    int disable_err = 0;

     
    len = strlen(base);
    if ((len > 499) || (len < 5)) {
	fprintf(stderr, "len(base) == %d !\n", len);
	return(-1);
    }
    len -= 4;  
    memcpy(prefix, base, len);
    prefix[len] = 0;

     
    if ((!strcmp(prefix, "tutor10_1")) || (!strcmp(prefix, "tutor10_2")) ||
        (!strcmp(prefix, "tutor3_2")) || (!strcmp(prefix, "307377")) ||
        (!strcmp(prefix, "tutor8_2")))
	disable_err = 1;

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
	reader = xmlReaderForFile(instance, NULL, options);
	if (reader == NULL) {
	    fprintf(stderr, "Failed to build reder for %s\n", instance);
	}
	if (disable_err == 1)
	    ret = streamProcessTest(instance, result, NULL, reader, filename,
	                            options);
	else
	    ret = streamProcessTest(instance, result, err, reader, filename,
	                            options);
	xmlFreeTextReader(reader);
	if (ret != 0) {
	    fprintf(stderr, "instance %s failed\n", instance);
	    res = ret;
	}
    }
    globfree(&globbuf);

    return(res);
}