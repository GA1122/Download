static rdfacontext* rdfa_create_new_element_context(rdfalist* context_stack)
{
   rdfacontext* parent_context = (rdfacontext*)
      context_stack->items[context_stack->num_items - 1]->data;
   rdfacontext* rval = rdfa_create_context(parent_context->base);


   rval->base = rdfa_replace_string(rval->base, parent_context->base);
   rdfa_init_context(rval);

#ifndef LIBRDFA_IN_RAPTOR
   rdfa_free_mapping(rval->uri_mappings);
   rval->uri_mappings = rdfa_copy_mapping(parent_context->uri_mappings);
#endif

   if(parent_context->language != NULL)
   {
      rval->language =
         rdfa_replace_string(rval->language, parent_context->language);
   }

   rval->default_graph_triple_callback =
      parent_context->default_graph_triple_callback;
   rval->processor_graph_triple_callback =
      parent_context->processor_graph_triple_callback;
   rval->buffer_filler_callback = parent_context->buffer_filler_callback;

   rval->bnode_count = parent_context->bnode_count;
   rval->underscore_colon_bnode_name =
      rdfa_replace_string(rval->underscore_colon_bnode_name,
                          parent_context->underscore_colon_bnode_name);
   rval->recurse = parent_context->recurse;
   rval->skip_element = 0;
   rval->callback_data = parent_context->callback_data;
   rval->xml_literal_namespaces_defined =
      parent_context->xml_literal_namespaces_defined;
   rval->xml_literal_xml_lang_defined =
      parent_context->xml_literal_xml_lang_defined;


   if(parent_context->skip_element == 0)
   {
      if(parent_context->new_subject != NULL)
      {
         rval->parent_subject = rdfa_replace_string(
            rval->parent_subject, parent_context->new_subject);
      }
      else
      {
         rval->parent_subject = rdfa_replace_string(
            rval->parent_subject, parent_context->parent_subject);
      }

      if(parent_context->current_object_resource != NULL)
      {
         rval->parent_object =
            rdfa_replace_string(
               rval->parent_object, parent_context->current_object_resource);
      }
      else if(parent_context->new_subject != NULL)
      {
         rval->parent_object =
            rdfa_replace_string(
               rval->parent_object, parent_context->new_subject);
      }
      else
      {
         rval->parent_object =
            rdfa_replace_string(
               rval->parent_object, parent_context->parent_subject);
      }

      if(rval->incomplete_triples != NULL)
      {
         rdfa_free_list(rval->incomplete_triples);
      }

      rval->incomplete_triples =
         rdfa_copy_list(parent_context->local_incomplete_triples);
   }
   else
   {
      rval->parent_subject = rdfa_replace_string(
         rval->parent_subject, parent_context->parent_subject);
      rval->parent_object = rdfa_replace_string(
         rval->parent_object, parent_context->parent_object);

      rdfa_free_list(rval->incomplete_triples);
      rval->incomplete_triples =
         rdfa_copy_list(parent_context->incomplete_triples);

      rdfa_free_list(rval->local_incomplete_triples);
      rval->local_incomplete_triples =
         rdfa_copy_list(parent_context->local_incomplete_triples);
   }

#ifdef LIBRDFA_IN_RAPTOR
   rval->base_uri = parent_context->base_uri;
   rval->sax2     = parent_context->sax2;
   rval->namespace_handler = parent_context->namespace_handler;
   rval->namespace_handler_user_data = parent_context->namespace_handler_user_data;
#endif

   return rval;
}
