raptor_libxml_sax_init(raptor_sax2* sax2)
{
  xmlSAXHandler *sax = &sax2->sax;

  sax->internalSubset = raptor_libxml_internalSubset;
  sax->isStandalone = raptor_libxml_isStandalone;
  sax->hasInternalSubset = raptor_libxml_hasInternalSubset;
  sax->hasExternalSubset = raptor_libxml_hasExternalSubset;
  sax->resolveEntity = raptor_libxml_resolveEntity;
  sax->getEntity = raptor_libxml_getEntity;
  sax->getParameterEntity = raptor_libxml_getParameterEntity;
  sax->entityDecl = raptor_libxml_entityDecl;
  sax->attributeDecl = NULL;  
  sax->elementDecl = NULL;  
  sax->notationDecl = NULL;  
  sax->unparsedEntityDecl = raptor_libxml_unparsedEntityDecl;
  sax->setDocumentLocator = raptor_libxml_set_document_locator;
  sax->startDocument = raptor_libxml_startDocument;
  sax->endDocument = raptor_libxml_endDocument;
  sax->startElement= raptor_sax2_start_element;
  sax->endElement= raptor_sax2_end_element;
  sax->reference = NULL;      
  sax->characters= raptor_sax2_characters;
  sax->cdataBlock= raptor_sax2_cdata;  
  sax->ignorableWhitespace= raptor_sax2_cdata;
  sax->processingInstruction = NULL;  
  sax->comment = raptor_sax2_comment;       
  sax->warning = (warningSAXFunc)raptor_libxml_warning;
  sax->error = (errorSAXFunc)raptor_libxml_error;
  sax->fatalError = (fatalErrorSAXFunc)raptor_libxml_fatal_error;
  sax->serror = (xmlStructuredErrorFunc)raptor_libxml_xmlStructuredError_handler_parsing;

#ifdef RAPTOR_LIBXML_XMLSAXHANDLER_EXTERNALSUBSET
  sax->externalSubset = raptor_libxml_externalSubset;
#endif

#ifdef RAPTOR_LIBXML_XMLSAXHANDLER_INITIALIZED
  sax->initialized = 1;
#endif
}