static void patternNode(FILE *out, xmlTextReaderPtr reader,
                        const char *pattern, xmlPatternPtr patternc,
			xmlStreamCtxtPtr patstream) {
    xmlChar *path = NULL;
    int match = -1;
    int type, empty;

    type = xmlTextReaderNodeType(reader);
    empty = xmlTextReaderIsEmptyElement(reader);

    if (type == XML_READER_TYPE_ELEMENT) {
	 
	match = xmlPatternMatch(patternc, xmlTextReaderCurrentNode(reader));

	if (match) {
	    path = xmlGetNodePath(xmlTextReaderCurrentNode(reader));
	    fprintf(out, "Node %s matches pattern %s\n", path, pattern);
	}
    }
    if (patstream != NULL) {
	int ret;

	if (type == XML_READER_TYPE_ELEMENT) {
	    ret = xmlStreamPush(patstream,
				xmlTextReaderConstLocalName(reader),
				xmlTextReaderConstNamespaceUri(reader));
	    if (ret < 0) {
		fprintf(out, "xmlStreamPush() failure\n");
		xmlFreeStreamCtxt(patstream);
		patstream = NULL;
	    } else if (ret != match) {
		if (path == NULL) {
		    path = xmlGetNodePath(
				   xmlTextReaderCurrentNode(reader));
		}
		fprintf(out,
			"xmlPatternMatch and xmlStreamPush disagree\n");
		fprintf(out,
			"  pattern %s node %s\n",
			pattern, path);
	    }


	}
	if ((type == XML_READER_TYPE_END_ELEMENT) ||
	    ((type == XML_READER_TYPE_ELEMENT) && (empty))) {
	    ret = xmlStreamPop(patstream);
	    if (ret < 0) {
		fprintf(out, "xmlStreamPop() failure\n");
		xmlFreeStreamCtxt(patstream);
		patstream = NULL;
	    }
	}
    }
    if (path != NULL)
	xmlFree(path);
}