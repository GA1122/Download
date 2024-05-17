   end_element(void *user_data, const char *name)
{
   rdfalist* context_stack = (rdfalist*)user_data;
   rdfacontext* context = (rdfacontext*)rdfa_pop_item(context_stack);
   rdfacontext* parent_context = (rdfacontext*)
      context_stack->items[context_stack->num_items - 1]->data;

   char* buffer = (char*)malloc(strlen(name) + 4);

   if(DEBUG)
   {
      printf("DEBUG: </%s>\n", name);
   }

   sprintf(buffer, "</%s>", name);
   if(context->xml_literal == NULL)
   {
      context->xml_literal =
         rdfa_replace_string(context->xml_literal, buffer);
      context->xml_literal_size = strlen(buffer);
   }
   else
   {
      context->xml_literal = rdfa_n_append_string(
         context->xml_literal, &context->xml_literal_size,
         buffer, strlen(buffer));
   }
   free(buffer);


   if(context->property != NULL)
   {
      char* saved_xml_literal = context->xml_literal;
      char* content_start = NULL;
      char* content_end = NULL;

      buffer = NULL;

      if(context->xml_literal != NULL)
      {
         content_start = strchr(context->xml_literal, '>');
         content_end = strrchr(context->xml_literal, '<');

         if((content_start != NULL) && (content_end != NULL))
         {
            context->xml_literal = ++content_start;
            *content_end = '\0';
         }
      }

      if(strlen(context->xml_literal) == 0)
      {
         context->plain_literal =
            rdfa_replace_string(context->plain_literal, "");
      }

      rdfa_complete_object_literal_triples(context);

      if(content_end != NULL)
      {
         *content_end = '<';
      }

      if(saved_xml_literal != NULL)
      {
         context->xml_literal = saved_xml_literal;
      }
   }


   if(context->xml_literal != NULL)
   {
      if(parent_context->xml_literal == NULL)
      {
         parent_context->xml_literal =
            rdfa_replace_string(
               parent_context->xml_literal, context->xml_literal);
         parent_context->xml_literal_size = context->xml_literal_size;
      }
      else
      {
         parent_context->xml_literal =
            rdfa_n_append_string(
               parent_context->xml_literal, &parent_context->xml_literal_size,
               context->xml_literal, context->xml_literal_size);
      }

      if(context->plain_literal != NULL)
      {
         if(parent_context->plain_literal == NULL)
         {
            parent_context->plain_literal =
               rdfa_replace_string(
                  parent_context->plain_literal, context->plain_literal);
            parent_context->plain_literal_size = context->plain_literal_size;
         }
         else
         {
            parent_context->plain_literal =
               rdfa_n_append_string(
                  parent_context->plain_literal,
                  &parent_context->plain_literal_size,
                  context->plain_literal,
                  context->plain_literal_size);
         }
      }
   }

   parent_context->bnode_count = context->bnode_count;
   parent_context->underscore_colon_bnode_name = \
      rdfa_replace_string(parent_context->underscore_colon_bnode_name,
                          context->underscore_colon_bnode_name);

   if((context->skip_element == 0) && (context->new_subject != NULL))
   {
      rdfa_complete_incomplete_triples(context);
   }

   rdfa_free_context(context);
}
