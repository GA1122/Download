xmlXPtrEvalChildSeq(xmlXPathParserContextPtr ctxt, xmlChar *name) {
     
    if ((name == NULL) && (CUR == '/') && (NXT(1) != '1')) {
        xmlXPtrErr(ctxt, XML_XPTR_CHILDSEQ_START,
		   "warning: ChildSeq not starting by /1\n", NULL);
    }

    if (name != NULL) {
	valuePush(ctxt, xmlXPathNewString(name));
	xmlFree(name);
	xmlXPathIdFunction(ctxt, 1);
	CHECK_ERROR;
    }

    while (CUR == '/') {
	int child = 0;
	NEXT;
        
	while ((CUR >= '0') && (CUR <= '9')) {
	    child = child * 10 + (CUR - '0');
	    NEXT;
	}
	xmlXPtrGetChildNo(ctxt, child);
    }
}
