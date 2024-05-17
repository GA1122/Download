htmlParseDocTypeDecl(htmlParserCtxtPtr ctxt) {
    const xmlChar *name;
    xmlChar *ExternalID = NULL;
    xmlChar *URI = NULL;

     
    SKIP(9);

    SKIP_BLANKS;

     
    name = htmlParseName(ctxt);
    if (name == NULL) {
	htmlParseErr(ctxt, XML_ERR_NAME_REQUIRED,
	             "htmlParseDocTypeDecl : no DOCTYPE name !\n",
		     NULL, NULL);
    }
     

    SKIP_BLANKS;

     
    URI = htmlParseExternalID(ctxt, &ExternalID);
    SKIP_BLANKS;

     
    if (CUR != '>') {
	htmlParseErr(ctxt, XML_ERR_DOCTYPE_NOT_FINISHED,
	             "DOCTYPE improperly terminated\n", NULL, NULL);
         
    }
    NEXT;

     
    if ((ctxt->sax != NULL) && (ctxt->sax->internalSubset != NULL) &&
	(!ctxt->disableSAX))
	ctxt->sax->internalSubset(ctxt->userData, name, ExternalID, URI);

     
    if (URI != NULL) xmlFree(URI);
    if (ExternalID != NULL) xmlFree(ExternalID);
}
