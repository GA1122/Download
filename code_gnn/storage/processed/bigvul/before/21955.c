static void rdfa_report_error(rdfacontext* context, char* data, size_t length)
{
   char* buffer = malloc(2<<12);
   snprintf(buffer, 2<<12, "XML parsing error: %s at line %d, column %d.",
      XML_ErrorString(XML_GetErrorCode(context->parser)),
      (int)XML_GetCurrentLineNumber(context->parser),
      (int)XML_GetCurrentColumnNumber(context->parser));

   if(context->processor_graph_triple_callback != NULL)
   {
      char* error_subject = rdfa_create_bnode(context);
      char* pointer_subject = rdfa_create_bnode(context);

      rdftriple* triple = rdfa_create_triple(
         error_subject, "http://www.w3.org/1999/02/22-rdf-syntax-ns#type",
         "http://www.w3.org/ns/rdfa_processing_graph#Error",
         RDF_TYPE_IRI, NULL, NULL);
      context->processor_graph_triple_callback(triple, context->callback_data);

      triple = rdfa_create_triple(
         error_subject, "http://purl.org/dc/terms/description", buffer,
         RDF_TYPE_PLAIN_LITERAL, NULL, "en");
      context->processor_graph_triple_callback(triple, context->callback_data);

      triple = rdfa_create_triple(
         error_subject, "http://www.w3.org/ns/rdfa_processing_graph#context",
         pointer_subject, RDF_TYPE_IRI, NULL, NULL);
      context->processor_graph_triple_callback(triple, context->callback_data);

      triple = rdfa_create_triple(
         pointer_subject, "http://www.w3.org/1999/02/22-rdf-syntax-ns#type",
         "http://www.w3.org/2009/pointers#LineCharPointer",
         RDF_TYPE_IRI, NULL, NULL);
      context->processor_graph_triple_callback(triple, context->callback_data);

      snprintf(buffer, 2<<12, "%d",
         (int)XML_GetCurrentLineNumber(context->parser));
      triple = rdfa_create_triple(
         pointer_subject, "http://www.w3.org/2009/pointers#lineNumber",
         buffer, RDF_TYPE_TYPED_LITERAL,
         "http://www.w3.org/2001/XMLSchema#positiveInteger", NULL);
      context->processor_graph_triple_callback(triple, context->callback_data);

      snprintf(buffer, 2<<12, "%d",
         (int)XML_GetCurrentColumnNumber(context->parser));
      triple = rdfa_create_triple(
         pointer_subject, "http://www.w3.org/2009/pointers#charNumber",
         buffer, RDF_TYPE_TYPED_LITERAL,
         "http://www.w3.org/2001/XMLSchema#positiveInteger", NULL);
      context->processor_graph_triple_callback(triple, context->callback_data);

      free(error_subject);
      free(pointer_subject);
   }
   else
   {
      printf("librdfa processor error: %s\n", buffer);
   }

   free(buffer);
}