int rdfa_parse_chunk(rdfacontext* context, char* data, size_t wblen, int done)
{
   if(context->done)
   {
      return RDFA_PARSE_FAILED;
   }

   if(!context->preread)
   {
      context->wb_preread = rdfa_init_base(context,
         &context->working_buffer, &context->wb_allocated, data, wblen);

      if(!context->base && context->wb_preread < (1<<17))
         return RDFA_PARSE_SUCCESS;

     rdfa_process_doctype(context, &wblen);

#ifdef LIBRDFA_IN_RAPTOR

      if(raptor_sax2_parse_chunk(context->sax2,
                                 (const unsigned char*)context->working_buffer,
                                 context->wb_position, done))
      {
         return RDFA_PARSE_FAILED;
      }
#else
      if(XML_Parse(context->parser, context->working_buffer,
         context->wb_position, 0) == XML_STATUS_ERROR)
      {
         rdfa_report_error(context, data, wblen);
         return RDFA_PARSE_FAILED;
      }
#endif

      context->preread = 1;

      return RDFA_PARSE_SUCCESS;
   }

#ifdef LIBRDFA_IN_RAPTOR
   if(raptor_sax2_parse_chunk(context->sax2, (const unsigned char*)data, wblen, done))
   {
      return RDFA_PARSE_FAILED;
   }
#else
   if(XML_Parse(context->parser, data, wblen, done) == XML_STATUS_ERROR)
   {
      rdfa_report_error(context, data, wblen);
      return RDFA_PARSE_FAILED;
   }
#endif

   return RDFA_PARSE_SUCCESS;
}
