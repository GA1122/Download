static void XMLCALL character_data(void *user_data, const char *s, int len)
{
   rdfalist* context_stack = (rdfalist*)user_data;
   rdfacontext* context = (rdfacontext*)
      context_stack->items[context_stack->num_items - 1]->data;

   char *buffer = (char*)malloc(len + 1);
   memset(buffer, 0, len + 1);
   memcpy(buffer, s, len);

   if(context->plain_literal == NULL)
   {
      context->plain_literal =
         rdfa_replace_string(context->plain_literal, buffer);
      context->plain_literal_size = len;
   }
   else
   {
      context->plain_literal = rdfa_n_append_string(
         context->plain_literal, &context->plain_literal_size, buffer, len);
   }

   if(context->xml_literal == NULL)
   {
      context->xml_literal =
         rdfa_replace_string(context->xml_literal, buffer);
      context->xml_literal_size = len;
   }
   else
   {
      context->xml_literal = rdfa_n_append_string(
         context->xml_literal, &context->xml_literal_size, buffer, len);
  }


   free(buffer);
}