static int rdfa_process_doctype(rdfacontext* context, size_t* bytes)
{
   int rval = 0;
   char* doctype_position = 0;
   char* doctype_buffer;
   const char* new_doctype =
      "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML+RDFa 1.0//EN\" "
      "\"http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd\">";

   doctype_buffer = (char*)malloc(*bytes + 2);
   memcpy(doctype_buffer, context->working_buffer, *bytes);
   doctype_buffer[*bytes + 1] = '\0';
   doctype_position = strstr(doctype_buffer, "<!DOCTYPE");

   if(doctype_position != NULL)
   {
      char* new_doctype_buffer = NULL;
      size_t new_doctype_buffer_length = 0;
      char* doctype_end = strchr(doctype_position, '>');

      if(doctype_end != NULL)
      {
         size_t bytes_to_copy = 0;
         size_t total_bytes = 0;

         bytes_to_copy = doctype_position - doctype_buffer;
         new_doctype_buffer = rdfa_n_append_string(new_doctype_buffer,
            &new_doctype_buffer_length, doctype_buffer, bytes_to_copy);
         total_bytes += bytes_to_copy;

         bytes_to_copy = RDFA_DOCTYPE_STRING_LENGTH;
         new_doctype_buffer = rdfa_n_append_string(new_doctype_buffer,
            &new_doctype_buffer_length, new_doctype, bytes_to_copy);
         total_bytes += bytes_to_copy;

         bytes_to_copy = *bytes - ((doctype_end + 1) - doctype_buffer);
         new_doctype_buffer = rdfa_n_append_string(new_doctype_buffer,
            &new_doctype_buffer_length, doctype_end + 1, bytes_to_copy);
         total_bytes += bytes_to_copy;

         free(context->working_buffer);
         context->working_buffer = new_doctype_buffer;
         context->wb_position = total_bytes;
         context->wb_allocated = total_bytes;
         *bytes = context->wb_allocated;

         rval = 1;
      }
   }
   else
   {
      char* new_doctype_buffer = NULL;
      size_t new_doctype_buffer_length = 0;

      char* html_position = strstr(doctype_buffer, "<html");
      if(html_position == NULL)
      {
         html_position = strstr(doctype_buffer, "<HTML");
      }

      if(html_position != NULL)
      {
         size_t bytes_to_copy = 0;
         size_t total_bytes = 0;

         bytes_to_copy = html_position - doctype_buffer;
         new_doctype_buffer = rdfa_n_append_string(new_doctype_buffer,
            &new_doctype_buffer_length, doctype_buffer, bytes_to_copy);
         total_bytes += bytes_to_copy;

         bytes_to_copy = RDFA_DOCTYPE_STRING_LENGTH;
         new_doctype_buffer = rdfa_n_append_string(new_doctype_buffer,
            &new_doctype_buffer_length, new_doctype, bytes_to_copy);
         total_bytes += bytes_to_copy;

         bytes_to_copy = 1;
         new_doctype_buffer = rdfa_n_append_string(new_doctype_buffer,
            &new_doctype_buffer_length, "\n", bytes_to_copy);
         total_bytes += bytes_to_copy;

         bytes_to_copy = *bytes - (html_position - doctype_buffer);
         new_doctype_buffer = rdfa_n_append_string(new_doctype_buffer,
            &new_doctype_buffer_length, html_position, bytes_to_copy);
         total_bytes += bytes_to_copy;

         free(context->working_buffer);
         context->working_buffer = new_doctype_buffer;
         context->wb_position = total_bytes;
         context->wb_allocated = total_bytes;
         *bytes = context->wb_allocated;

         rval = 1;
      }
   }

   free(doctype_buffer);

   return rval;
}