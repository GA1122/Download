raptor_libxml_update_document_locator(raptor_sax2* sax2,
                                      raptor_locator* locator)
{
   
  xmlSAXLocatorPtr loc = sax2 ? sax2->loc : NULL;
  xmlParserCtxtPtr xc= sax2 ? sax2->xc : NULL;

  if(xc && xc->inSubset)
    return;

  if(!locator) 
    return;
  
  locator->line= -1;
  locator->column= -1;

  if(!xc)
    return;

  if(loc) {
    locator->line = loc->getLineNumber(xc);
     
     
  }

}