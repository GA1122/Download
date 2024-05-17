static size_t rdfa_init_base(
   rdfacontext* context, char** working_buffer, size_t* working_buffer_size,
   char* temp_buffer, size_t bytes_read)
{
   char* head_end = NULL;
   size_t offset = context->wb_position;
   size_t needed_size = (offset + bytes_read) - *working_buffer_size;


   if(needed_size > 0)
   {
      size_t temp_buffer_size = sizeof(char) * READ_BUFFER_SIZE;
      if((size_t)needed_size > temp_buffer_size)
         temp_buffer_size += needed_size;

      *working_buffer_size += temp_buffer_size;
      *working_buffer = (char*)realloc(*working_buffer, *working_buffer_size + 1);
   }

   memmove(*working_buffer + offset, temp_buffer, bytes_read);
   *(*working_buffer + offset + bytes_read) = '\0';

   head_end = strstr(*working_buffer, "</head>");
   if(head_end == NULL)
      head_end = strstr(*working_buffer, "</HEAD>");

   context->wb_position += bytes_read;

   if(head_end == NULL)
      return bytes_read;

   if(head_end != NULL)
   {
      char* base_start = strstr(*working_buffer, "<base ");
      if(base_start == NULL)
         base_start = strstr(*working_buffer, "<BASE ");

      if(base_start != NULL)
      {
         char* href_start = strstr(base_start, "href=");
         char sep = href_start[5];
         char* uri_start = href_start + 6;
         char* uri_end = strchr(uri_start, sep);

         if((uri_start != NULL) && (uri_end != NULL))
         {
            if(*uri_start != sep)
            {
               size_t uri_size = uri_end - uri_start;
               char* temp_uri = (char*)malloc(sizeof(char) * uri_size + 1);
	       char* cleaned_base;
               strncpy(temp_uri, uri_start, uri_size);
               temp_uri[uri_size] = '\0';

	       cleaned_base = rdfa_iri_get_base(temp_uri);
               context->current_object_resource =
                  rdfa_replace_string(
                     context->current_object_resource, cleaned_base);

               context->base =
                  rdfa_replace_string(context->base, cleaned_base);
               free(cleaned_base);
               free(temp_uri);
            }
         }
      }
   }

   return bytes_read;
}
