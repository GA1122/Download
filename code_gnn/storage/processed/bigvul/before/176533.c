xmlPrintURI(FILE *stream, xmlURIPtr uri) {
    xmlChar *out;

    out = xmlSaveUri(uri);
 if (out != NULL) {
	fprintf(stream, "%s", (char *) out);
	xmlFree(out);
 }
}
