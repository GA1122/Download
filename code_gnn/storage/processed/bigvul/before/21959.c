   start_element(void* user_data, const char* name, const char** attributes)
{
   rdfalist* context_stack = (rdfalist*) user_data;
   rdfacontext* context = rdfa_create_new_element_context(context_stack);
   const char** aptr = attributes;
   const char* xml_lang = NULL;
   const char* about_curie = NULL;
   char* about = NULL;
   const char* src_curie = NULL;
   char* src = NULL;
   const char* type_of_curie = NULL;
   rdfalist* type_of = NULL;
   const char* rel_curie = NULL;
   rdfalist* rel = NULL;
   const char* rev_curie = NULL;
   rdfalist* rev = NULL;
   const char* property_curie = NULL;
   rdfalist* property = NULL;
   const char* resource_curie = NULL;
   char* resource = NULL;
   const char* href_curie = NULL;
   char* href = NULL;
   const char* content = NULL;
   const char* datatype_curie = NULL;
   char* datatype = NULL;

   rdfa_push_item(context_stack, context, RDFALIST_FLAG_CONTEXT);

   if(DEBUG)
   {
      printf("DEBUG: ------- START - %s -------\n", name);
   }

   if(context->xml_literal == NULL)
   {
      context->xml_literal = rdfa_replace_string(context->xml_literal, "<");
      context->xml_literal_size = 1;
   }
   else
   {
      context->xml_literal = rdfa_n_append_string(
         context->xml_literal, &context->xml_literal_size, "<", 1);
   }
   context->xml_literal = rdfa_n_append_string(
      context->xml_literal, &context->xml_literal_size,
      name, strlen(name));

   if(!context->xml_literal_namespaces_defined)
   {
#ifdef LIBRDFA_IN_RAPTOR
      raptor_namespace_stack* nstack = &context->sax2->namespaces;
      raptor_namespace* ns;
      raptor_namespace** ns_list = NULL;
      size_t ns_size;
#else
      char** umap = context->uri_mappings;
#endif
      char* umap_key = NULL;
      char* umap_value = NULL;

      context->xml_literal_xml_lang_defined = 0;

#ifdef LIBRDFA_IN_RAPTOR
      ns_size = 0;
      ns_list = raptor_namespace_stack_to_array(nstack, &ns_size);
      qsort((void*)ns_list, ns_size, sizeof(raptor_namespace*),
            raptor_nspace_compare);

      while(ns_size > 0)
#else
      while(*umap != NULL)
#endif
      {
         unsigned char insert_xmlns_definition = 1;
         const char* attr = NULL;

#ifdef LIBRDFA_IN_RAPTOR
         ns=ns_list[--ns_size];

         umap_key = (char*)raptor_namespace_get_prefix(ns);
         if(!umap_key)
           umap_key=(char*)XMLNS_DEFAULT_MAPPING;
         umap_value = (char*)raptor_uri_as_string(raptor_namespace_get_uri(ns));
#else
         rdfa_next_mapping(umap++, &umap_key, &umap_value);
         umap++;
#endif

         if(attributes != NULL)
         {
            const char** attrs = attributes;
            while((*attrs != NULL) && insert_xmlns_definition)
            {
               attr = *attrs++;

               if((strcmp(attr, umap_key) == 0) ||
                  (strcmp(umap_key, XMLNS_DEFAULT_MAPPING) == 0))
               {
                  insert_xmlns_definition = 0;
               }
            }
         }

         if(insert_xmlns_definition)
         {
            context->xml_literal = rdfa_n_append_string(
               context->xml_literal, &context->xml_literal_size,
               " xmlns", strlen(" xmlns"));

            if(strcmp(umap_key, XMLNS_DEFAULT_MAPPING) != 0)
            {
               context->xml_literal = rdfa_n_append_string(
                  context->xml_literal, &context->xml_literal_size, ":", 1);
               context->xml_literal = rdfa_n_append_string(
                  context->xml_literal, &context->xml_literal_size,
                  umap_key, strlen(umap_key));
            }

            context->xml_literal = rdfa_n_append_string(
               context->xml_literal, &context->xml_literal_size, "=\"", 2);
            context->xml_literal = rdfa_n_append_string(
               context->xml_literal, &context->xml_literal_size,
               umap_value, strlen(umap_value));
            context->xml_literal = rdfa_n_append_string(
               context->xml_literal, &context->xml_literal_size, "\"", 1);
         }

      }  
      context->xml_literal_namespaces_defined = 1;

#ifdef LIBRDFA_IN_RAPTOR
      if(ns_list)
        raptor_free_memory(ns_list);
#endif
   }  


   if(aptr != NULL)
   {
      while(*aptr != NULL)
      {
         const char* attr;
         const char* value;
         char* literal_text;

         attr = *aptr++;
         value = *aptr++;

         literal_text = (char*)malloc(strlen(attr) + strlen(value) + 5);
         sprintf(literal_text, " %s=\"%s\"", attr, value);
         context->xml_literal = rdfa_n_append_string(
            context->xml_literal, &context->xml_literal_size,
            literal_text, strlen(literal_text));
         free(literal_text);

         if(strcmp(attr, "xml:lang") == 0)
         {
            context->xml_literal_xml_lang_defined = 1;
         }

         if(strcmp(attr, "about") == 0)
         {
            about_curie = value;
            about = rdfa_resolve_curie(
               context, about_curie, CURIE_PARSE_ABOUT_RESOURCE);
         }
         else if(strcmp(attr, "src") == 0)
         {
            src_curie = value;
            src = rdfa_resolve_curie(context, src_curie, CURIE_PARSE_HREF_SRC);
         }
         else if(strcmp(attr, "typeof") == 0)
         {
            type_of_curie = value;
            type_of = rdfa_resolve_curie_list(
               context, type_of_curie,
               CURIE_PARSE_INSTANCEOF_DATATYPE);
         }
         else if(strcmp(attr, "rel") == 0)
         {
            rel_curie = value;
            rel = rdfa_resolve_curie_list(
               context, rel_curie, CURIE_PARSE_RELREV);
         }
         else if(strcmp(attr, "rev") == 0)
         {
            rev_curie = value;
            rev = rdfa_resolve_curie_list(
               context, rev_curie, CURIE_PARSE_RELREV);
         }
         else if(strcmp(attr, "property") == 0)
         {
            property_curie = value;
            property =
               rdfa_resolve_curie_list(
                  context, property_curie, CURIE_PARSE_PROPERTY);
         }
         else if(strcmp(attr, "resource") == 0)
         {
            resource_curie = value;
            resource = rdfa_resolve_curie(
               context, resource_curie, CURIE_PARSE_ABOUT_RESOURCE);
         }
         else if(strcmp(attr, "href") == 0)
         {
            href_curie = value;
            href =
               rdfa_resolve_curie(context, href_curie, CURIE_PARSE_HREF_SRC);
         }
         else if(strcmp(attr, "content") == 0)
         {
            content = value;
         }
         else if(strcmp(attr, "datatype") == 0)
         {
            datatype_curie = value;

            if(strlen(datatype_curie) == 0)
            {
               datatype = rdfa_replace_string(datatype, "");
            }
            else
            {
               datatype = rdfa_resolve_curie(context, datatype_curie,
                  CURIE_PARSE_INSTANCEOF_DATATYPE);
            }
         }
#ifndef LIBRDFA_IN_RAPTOR
         else if(strcmp(attr, "xml:lang") == 0)
         {
            xml_lang = value;
         }
         else if(strstr(attr, "xmlns") != NULL)
         {
            rdfa_update_uri_mappings(context, attr, value);
         }
#endif
      }
   }

#ifdef LIBRDFA_IN_RAPTOR
   if(context->sax2) {
      xml_lang = (const char*)raptor_sax2_inscope_xml_language(context->sax2);
      if(!xml_lang)
        xml_lang = "";
   }
#endif
   if((xml_lang == NULL) && (context->language != NULL) &&
      !context->xml_literal_xml_lang_defined)
   {
      context->xml_literal = rdfa_n_append_string(
         context->xml_literal, &context->xml_literal_size,
         " xml:lang=\"", strlen(" xml:lang=\""));
      context->xml_literal = rdfa_n_append_string(
         context->xml_literal, &context->xml_literal_size,
         context->language, strlen(context->language));
      context->xml_literal = rdfa_n_append_string(
         context->xml_literal, &context->xml_literal_size, "\"", 1);

      context->xml_literal_xml_lang_defined = 1;
   }

   context->xml_literal = rdfa_n_append_string(
      context->xml_literal, &context->xml_literal_size, ">", 1);

   rdfa_update_language(context, xml_lang);

    
   if(DEBUG)
   {
      if(about != NULL)
      {
         printf("DEBUG: @about = %s\n", about);
      }
      if(src != NULL)
      {
         printf("DEBUG: @src = %s\n", src);
      }
      if(type_of != NULL)
      {
         printf("DEBUG: @type_of = ");
         rdfa_print_list(type_of);
      }
      if(rel != NULL)
      {
         printf("DEBUG: @rel = ");
         rdfa_print_list(rel);
      }
      if(rev != NULL)
      {
         printf("DEBUG: @rev = ");
         rdfa_print_list(rev);
      }
      if(property != NULL)
      {
         printf("DEBUG: @property = ");
         rdfa_print_list(property);
      }
      if(resource != NULL)
      {
         printf("DEBUG: @resource = %s\n", resource);
      }
      if(href != NULL)
      {
         printf("DEBUG: @href = %s\n", href);
      }
      if(content != NULL)
      {
         printf("DEBUG: @content = %s\n", content);
      }
      if(datatype != NULL)
      {
         printf("DEBUG: @datatype = %s\n", datatype);
      }
   }

   if((about == NULL) && (src == NULL) && (type_of == NULL) &&
      (rel == NULL) && (rev == NULL) && (property == NULL) &&
      (resource == NULL) && (href == NULL))
   {
      context->skip_element = 1;
   }

   if((rel == NULL) && (rev == NULL))
   {
      rdfa_establish_new_subject(
         context, name, about, src, resource, href, type_of);
   }
   else
   {
      rdfa_establish_new_subject_with_relrev(
         context, name, about, src, resource, href, type_of);
   }

   if(context->new_subject != NULL)
   {
      if(DEBUG)
      {
         printf("DEBUG: new_subject = %s\n", context->new_subject);
      }


      if(type_of != NULL)
      {
         rdfa_complete_type_triples(context, type_of);
      }

   }

   if(context->current_object_resource != NULL)
   {
      rdfa_complete_relrev_triples(context, rel, rev);
   }
   else if((rel != NULL) || (rev != NULL))
   {
      rdfa_save_incomplete_triples(context, rel, rev);
   }

   if(property != NULL)
   {
      context->xml_literal_namespaces_defined = 0;
   }

   context->property = property;
   context->content = rdfa_replace_string(context->datatype, content);
   context->datatype = rdfa_replace_string(context->datatype, datatype);

   free(about);
   free(src);
   rdfa_free_list(type_of);
   rdfa_free_list(rel);
   rdfa_free_list(rev);
   free(resource);
   free(href);
   free(datatype);
}
