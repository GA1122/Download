xmlParseEndTag2(xmlParserCtxtPtr ctxt, const xmlChar *prefix,
 const xmlChar *URI, int line, int nsNr, int tlen) {
 const xmlChar *name;
 size_t curLength;

    GROW;
 if ((RAW != '<') || (NXT(1) != '/')) {
	xmlFatalErr(ctxt, XML_ERR_LTSLASH_REQUIRED, NULL);
 return;
 }
    SKIP(2);

    curLength = ctxt->input->end - ctxt->input->cur;
 if ((tlen > 0) && (curLength >= (size_t)tlen) &&
 (xmlStrncmp(ctxt->input->cur, ctxt->name, tlen) == 0)) {
 if ((curLength >= (size_t)(tlen + 1)) &&
 (ctxt->input->cur[tlen] == '>')) {
	    ctxt->input->cur += tlen + 1;
	    ctxt->input->col += tlen + 1;
 goto done;
 }
	ctxt->input->cur += tlen;
	ctxt->input->col += tlen;
	name = (xmlChar*)1;
 } else {
 if (prefix == NULL)
	    name = xmlParseNameAndCompare(ctxt, ctxt->name);
 else
	    name = xmlParseQNameAndCompare(ctxt, ctxt->name, prefix);
 }

  
    GROW;
 if (ctxt->instate == XML_PARSER_EOF)
 return;
    SKIP_BLANKS;
 if ((!IS_BYTE_CHAR(RAW)) || (RAW != '>')) {
	xmlFatalErr(ctxt, XML_ERR_GT_REQUIRED, NULL);
 } else
	NEXT1;

  
 if (name != (xmlChar*)1) {
 if (name == NULL) name = BAD_CAST "unparseable";
 if ((line == 0) && (ctxt->node != NULL))
            line = ctxt->node->line;
        xmlFatalErrMsgStrIntStr(ctxt, XML_ERR_TAG_NAME_MISMATCH,
 "Opening and ending tag mismatch: %s line %d and %s\n",
		                ctxt->name, line, name);
 }

  
done:
 if ((ctxt->sax != NULL) && (ctxt->sax->endElementNs != NULL) &&
 (!ctxt->disableSAX))
	ctxt->sax->endElementNs(ctxt->userData, ctxt->name, prefix, URI);

    spacePop(ctxt);
 if (nsNr != 0)
	nsPop(ctxt, nsNr);
 return;
}