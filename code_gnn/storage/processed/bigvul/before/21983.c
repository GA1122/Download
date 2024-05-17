raptor_libxml_xmlStructuredError_handler_parsing(void *user_data,
                                                 xmlErrorPtr err)
{
  raptor_sax2* sax2 = NULL;

   
  if(user_data) {
    sax2 = (raptor_sax2*)user_data;
    if(sax2->magic != RAPTOR_LIBXML_MAGIC)
      sax2 = NULL;
  }
  
   
  if(err && err->ctxt) {
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr)err->ctxt;
    if(ctxt->userData) {
      sax2 = (raptor_sax2*)ctxt->userData;
      if(sax2->magic != RAPTOR_LIBXML_MAGIC)
        sax2 = NULL;
    }
  }

  if(sax2)
    raptor_libxml_xmlStructuredError_handler_common(sax2->world, sax2->locator,
                                                    err);
  else
    raptor_libxml_xmlStructuredError_handler_common(NULL, NULL, err);
}
