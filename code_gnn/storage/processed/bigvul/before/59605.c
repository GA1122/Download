oldParseTest(const char *filename, const char *result,
             const char *err ATTRIBUTE_UNUSED,
	     int options ATTRIBUTE_UNUSED) {
    xmlDocPtr doc;
    char *temp;
    int res = 0;

    nb_tests++;
     
#ifdef LIBXML_SAX1_ENABLED
    doc = xmlParseFile(filename);
#else
    doc = xmlReadFile(filename, NULL, 0);
#endif
    if (doc == NULL)
        return(1);
    temp = resultFilename(filename, "", ".res");
    if (temp == NULL) {
        fprintf(stderr, "out of memory\n");
        fatalError();
    }
    xmlSaveFile(temp, doc);
    if (compareFiles(temp, result)) {
        res = 1;
    }
    xmlFreeDoc(doc);

     
#ifdef LIBXML_SAX1_ENABLED
    doc = xmlParseFile(temp);
#else
    doc = xmlReadFile(temp, NULL, 0);
#endif
    if (doc == NULL)
        return(1);
    xmlSaveFile(temp, doc);
    if (compareFiles(temp, result)) {
        res = 1;
    }
    xmlFreeDoc(doc);

    if (temp != NULL) {
        unlink(temp);
        free(temp);
    }
    return(res);
}
