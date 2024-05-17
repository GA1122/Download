static void rdfa_free_context_stack(rdfacontext* context)
{
   if(context->context_stack != NULL)
   {
      void* rval;
      do
      {
         rval = rdfa_pop_item(context->context_stack);
         if(rval && rval != context)
         {
            rdfa_free_context((rdfacontext*)rval);
         }
      }
      while(rval);
      free(context->context_stack->items);
      free(context->context_stack);
      context->context_stack = NULL;
   }
}
