xmlCreateEntityParserCtxtInternal(const xmlChar *URL, const xmlChar *ID,
 const xmlChar *base, xmlParserCtxtPtr pctx) {
    xmlParserCtxtPtr ctxt;
    xmlParserInputPtr inputStream;
 char *directory = NULL;
    xmlChar *uri;

    ctxt = xmlNewParserCtxt();
 if (ctxt == NULL) {
 return(NULL);
 }

 if (pctx != NULL) {
        ctxt->options = pctx->options;
        ctxt->_private = pctx->_private;
 }

    uri = xmlBuildURI(URL, base);

 if (uri == NULL) {
	inputStream = xmlLoadExternalEntity((char *)URL, (char *)ID, ctxt);
 if (inputStream == NULL) {
	    xmlFreeParserCtxt(ctxt);
 return(NULL);
 }

	inputPush(ctxt, inputStream);

 if ((ctxt->directory == NULL) && (directory == NULL))
	    directory = xmlParserGetDirectory((char *)URL);
 if ((ctxt->directory == NULL) && (directory != NULL))
	    ctxt->directory = directory;
 } else {
	inputStream = xmlLoadExternalEntity((char *)uri, (char *)ID, ctxt);
 if (inputStream == NULL) {
	    xmlFree(uri);
	    xmlFreeParserCtxt(ctxt);
 return(NULL);
 }

	inputPush(ctxt, inputStream);

 if ((ctxt->directory == NULL) && (directory == NULL))
	    directory = xmlParserGetDirectory((char *)uri);
 if ((ctxt->directory == NULL) && (directory != NULL))
	    ctxt->directory = directory;
	xmlFree(uri);
 }
 return(ctxt);
}