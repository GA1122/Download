xmlSAXParseEntity(xmlSAXHandlerPtr sax, const char *filename) {
    xmlDocPtr ret;
    xmlParserCtxtPtr ctxt;

    ctxt = xmlCreateFileParserCtxt(filename);
 if (ctxt == NULL) {
 return(NULL);
 }
 if (sax != NULL) {
 if (ctxt->sax != NULL)
	    xmlFree(ctxt->sax);
        ctxt->sax = sax;
        ctxt->userData = NULL;
 }

    xmlParseExtParsedEnt(ctxt);

 if (ctxt->wellFormed)
	ret = ctxt->myDoc;
 else {
        ret = NULL;
        xmlFreeDoc(ctxt->myDoc);
        ctxt->myDoc = NULL;
 }
 if (sax != NULL)
        ctxt->sax = NULL;
    xmlFreeParserCtxt(ctxt);

 return(ret);
}