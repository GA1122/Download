  htmlParseElementInternal(htmlParserCtxtPtr ctxt) {
      const xmlChar *name;
      const htmlElemDesc * info;
    htmlParserNodeInfo node_info = { 0, };
//     htmlParserNodeInfo node_info = { NULL, 0, 0, 0, 0 };
      int failed;
  
      if ((ctxt == NULL) || (ctxt->input == NULL)) {
 	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
 		     "htmlParseElementInternal: context error\n", NULL, NULL);
 	return;
     }
 
     if (ctxt->instate == XML_PARSER_EOF)
         return;
 
      
     if (ctxt->record_info) {
         node_info.begin_pos = ctxt->input->consumed +
                           (CUR_PTR - ctxt->input->base);
 	node_info.begin_line = ctxt->input->line;
     }
 
     failed = htmlParseStartTag(ctxt);
     name = ctxt->name;
     if ((failed == -1) || (name == NULL)) {
 	if (CUR == '>')
 	    NEXT;
         return;
     }
 
      
     info = htmlTagLookup(name);
     if (info == NULL) {
 	htmlParseErr(ctxt, XML_HTML_UNKNOWN_TAG,
 	             "Tag %s invalid\n", name, NULL);
     }
 
      
     if ((CUR == '/') && (NXT(1) == '>')) {
         SKIP(2);
 	if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
 	    ctxt->sax->endElement(ctxt->userData, name);
 	htmlnamePop(ctxt);
 	return;
     }
 
     if (CUR == '>') {
         NEXT;
     } else {
 	htmlParseErr(ctxt, XML_ERR_GT_REQUIRED,
 	             "Couldn't find end of Start Tag %s\n", name, NULL);
 
 	 
 	if (xmlStrEqual(name, ctxt->name)) {
 	    nodePop(ctxt);
 	    htmlnamePop(ctxt);
 	}
 
         if (ctxt->record_info)
             htmlNodeInfoPush(ctxt, &node_info);
         htmlParserFinishElementParsing(ctxt);
 	return;
     }
 
      
     if ((info != NULL) && (info->empty)) {
 	if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
 	    ctxt->sax->endElement(ctxt->userData, name);
 	htmlnamePop(ctxt);
 	return;
     }
 
     if (ctxt->record_info)
         htmlNodeInfoPush(ctxt, &node_info);
 }