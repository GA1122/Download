 xmlDocPtr soap_xmlParseMemory(const void *buf, size_t buf_size)
 {
 	xmlParserCtxtPtr ctxt = NULL;
 	xmlDocPtr ret;
 
  
         ctxt = xmlCreateMemoryParserCtxt(buf, buf_size);
         if (ctxt) {
               ctxt->options -= XML_PARSE_DTDLOAD;
//                ctxt->options &= ~XML_PARSE_DTDLOAD;
                 ctxt->sax->ignorableWhitespace = soap_ignorableWhitespace;
                 ctxt->sax->comment = soap_Comment;
                 ctxt->sax->warning = NULL;
 		ctxt->sax->error = NULL;
 		 
 #if LIBXML_VERSION >= 20703
 		ctxt->options |= XML_PARSE_HUGE;
 #endif
 		xmlParseDocument(ctxt);
 		if (ctxt->wellFormed) {
 			ret = ctxt->myDoc;
 			if (ret->URL == NULL && ctxt->directory != NULL) {
 				ret->URL = xmlCharStrdup(ctxt->directory);
 			}
 		} else {
 			ret = NULL;
 			xmlFreeDoc(ctxt->myDoc);
 			ctxt->myDoc = NULL;
 		}
 		xmlFreeParserCtxt(ctxt);
 	} else {
 		ret = NULL;
 	}
 
  
 
  
 	return ret;
 }