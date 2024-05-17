noentParseTest(const char *filename, const char *result,
               const char *err  ATTRIBUTE_UNUSED,
	       int options) {
    xmlDocPtr doc;
    char *temp;
    int res = 0;

    nb_tests++;
     
    doc = xmlReadFile(filename, NULL, options);
    if (doc == NULL)
        return(1);
    temp = resultFilename(filename, "", ".res");
    if (temp == NULL) {
        fprintf(stderr, "Out of memory\n");
        fatalError();
    }
    xmlSaveFile(temp, doc);
    if (compareFiles(temp, result)) {
        res = 1;
    }
    xmlFreeDoc(doc);

     
    doc = xmlReadFile(filename, NULL, options);
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
