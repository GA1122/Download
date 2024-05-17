void rdfa_free_context(rdfacontext* context)
{
   free(context->base);
   free(context->parent_subject);
   free(context->parent_object);

#ifndef LIBRDFA_IN_RAPTOR
   rdfa_free_mapping(context->uri_mappings);
#endif

   rdfa_free_list(context->incomplete_triples);
   free(context->language);
   free(context->underscore_colon_bnode_name);
   free(context->new_subject);
   free(context->current_object_resource);
   free(context->content);
   free(context->datatype);
   rdfa_free_list(context->property);
   free(context->plain_literal);
   free(context->xml_literal);

   rdfa_free_list(context->local_incomplete_triples);

   rdfa_free_context_stack(context);
   free(context->working_buffer);
   free(context);
}