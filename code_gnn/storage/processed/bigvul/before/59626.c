streamMemParseTest(const char *filename, const char *result, const char *err,
                   int options) {
    xmlTextReaderPtr reader;
    int ret;
    const char *base;
    int size;

     
    if (loadMem(filename, &base, &size) != 0) {
        fprintf(stderr, "Failed to load %s\n", filename);
	return(-1);
    }
    reader = xmlReaderForMemory(base, size, filename, NULL, options);
    ret = streamProcessTest(filename, result, err, reader, NULL, options);
    free((char *)base);
    xmlFreeTextReader(reader);
    return(ret);
}