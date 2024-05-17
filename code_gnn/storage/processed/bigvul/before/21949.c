void rdfa_init_context(rdfacontext* context)
{
   context->parent_subject = NULL;
   if(context->base != NULL)
   {
      char* cleaned_base = rdfa_iri_get_base(context->base);
      context->parent_subject =
         rdfa_replace_string(context->parent_subject, cleaned_base);
      free(cleaned_base);
   }

   context->parent_object = NULL;

#ifndef LIBRDFA_IN_RAPTOR
   context->uri_mappings = (char**)rdfa_create_mapping(MAX_URI_MAPPINGS);
#endif

   context->incomplete_triples = rdfa_create_list(3);

   context->language = NULL;

   context->current_object_resource = NULL;

   context->recurse = 1;

   context->skip_element = 0;

   context->new_subject = NULL;

   context->current_object_resource = NULL;


   context->local_incomplete_triples = rdfa_create_list(3);


   context->bnode_count = 0;
   context->underscore_colon_bnode_name = NULL;
   context->xml_literal_namespaces_defined = 0;
   context->xml_literal_xml_lang_defined = 0;
   context->content = NULL;
   context->datatype = NULL;
   context->property = NULL;
   context->plain_literal = NULL;
   context->plain_literal_size = 0;
   context->xml_literal = NULL;
   context->xml_literal_size = 0;
}
