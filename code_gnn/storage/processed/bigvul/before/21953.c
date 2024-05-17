void rdfa_parse_end(rdfacontext* context)
{
   rdfa_free_context_stack(context);

#ifdef LIBRDFA_IN_RAPTOR
   if(context->base_uri)
      raptor_free_uri(context->base_uri);
   raptor_free_sax2(context->sax2);
   context->sax2=NULL;
#else
   XML_ParserFree(context->parser);
#endif
}
