xmlParseNameAndCompare(xmlParserCtxtPtr ctxt, xmlChar const *other) {
 register const xmlChar *cmp = other;
 register const xmlChar *in;
 const xmlChar *ret;

    GROW;
 if (ctxt->instate == XML_PARSER_EOF)
 return(NULL);

    in = ctxt->input->cur;
 while (*in != 0 && *in == *cmp) {
 ++in;
 ++cmp;
	ctxt->input->col++;
 }
 if (*cmp == 0 && (*in == '>' || IS_BLANK_CH (*in))) {
  
	ctxt->input->cur = in;
 return (const xmlChar*) 1;
 }
  
    ret = xmlParseName (ctxt);
  
 if (ret == other) {
 return (const xmlChar*) 1;
 }
 return ret;
}
