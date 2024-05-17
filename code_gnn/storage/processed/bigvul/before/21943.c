rdfacontext* rdfa_create_context(const char* base)
{
   rdfacontext* rval = NULL;
   size_t base_length = strlen(base);

   if(base_length > 0)
   {
      char* cleaned_base;
      rval = (rdfacontext*)malloc(sizeof(rdfacontext));
      rval->base = NULL;
      cleaned_base = rdfa_iri_get_base(base);
      rval->base = rdfa_replace_string(rval->base, cleaned_base);
      free(cleaned_base);

      rval->default_graph_triple_callback = NULL;
      rval->buffer_filler_callback = NULL;
      rval->processor_graph_triple_callback = NULL;
      rval->callback_data = NULL;

       
      rval->wb_allocated = 0;
      rval->working_buffer = NULL;
      rval->wb_position = 0;
#ifdef LIBRDFA_IN_RAPTOR
      rval->base_uri = NULL;
      rval->sax2 = NULL;
      rval->namespace_handler = NULL;
      rval->namespace_handler_user_data = NULL;
#else
      rval->uri_mappings = NULL;
      rval->parser = NULL;
#endif
      rval->done = 0;
      rval->context_stack = NULL;
      rval->wb_preread = 0;
      rval->preread = 0;
   }
   else
   {
      printf("librdfa error: Failed to create a parsing context, "
         "base IRI was not specified!\n");
   }

   return rval;
}