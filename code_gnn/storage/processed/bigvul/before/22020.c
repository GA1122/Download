raptor_rdfxml_process_property_attributes(raptor_parser *rdf_parser, 
                                          raptor_rdfxml_element *attributes_element,
                                          raptor_rdfxml_element *resource_element,
                                          raptor_term *property_node_identifier)
{
  unsigned int i;
  raptor_term *resource_identifier;

  resource_identifier = property_node_identifier ? property_node_identifier : resource_element->subject;
  

   
  for(i = 0; i < attributes_element->xml_element->attribute_count; i++) {
    raptor_qname* attr = attributes_element->xml_element->attributes[i];
    const unsigned char *name;
    const unsigned char *value;
    int handled = 0;

    if(!attr)
      continue;
    
    name = attr->local_name;
    value = attr->value;

    if(!attr->nspace) {
      raptor_rdfxml_update_document_locator(rdf_parser);
      raptor_parser_error(rdf_parser,
                          "Using property attribute '%s' without a namespace is forbidden.",
                          name);
      continue;
    }


    if(!raptor_unicode_check_utf8_nfc_string(value, strlen((const char*)value),
                                             NULL)) {
      const char *message;

      message = "Property attribute '%s' has a string not in Unicode Normal Form C: %s";
      raptor_rdfxml_update_document_locator(rdf_parser);
      if(RAPTOR_OPTIONS_GET_NUMERIC(rdf_parser, RAPTOR_OPTION_NON_NFC_FATAL))
        raptor_parser_error(rdf_parser, message, name, value);
      else
        raptor_parser_warning(rdf_parser, message, name, value);
      continue;
    }
    

     
    if(attr->nspace->is_rdf_ms) {
       
        
      if(*name == '_') {
        int ordinal;

         
        name++;
        ordinal = raptor_check_ordinal(name);
        if(ordinal < 1) {
          raptor_rdfxml_update_document_locator(rdf_parser);
          raptor_parser_error(rdf_parser,
                              "Illegal ordinal value %d in property attribute '%s' seen on containing element '%s'.",
                              ordinal, attr->local_name, name);
        }
      } else {
        int rc;

        raptor_rdfxml_update_document_locator(rdf_parser);

        rc = raptor_rdfxml_check_propertyAttribute_name((const char*)name);
        if(!rc)
          raptor_parser_error(rdf_parser,
                              "RDF term %s is forbidden as a property attribute.",
                              name);
        else if(rc < 0)
          raptor_parser_warning(rdf_parser,
                                "Unknown RDF namespace property attribute '%s'.", 
                                name);
      }

    }  


    if(!handled) {
      raptor_term* object_term;

      object_term = raptor_new_term_from_literal(rdf_parser->world,
                                                 (unsigned char*)value,
                                                 NULL, NULL);
    
       
      raptor_rdfxml_generate_statement(rdf_parser, 
                                       resource_identifier,
                                       attr->uri,
                                       object_term,
                                       NULL,  
                                       resource_element);

      raptor_free_term(object_term);
    }
    
  }  


   
  for(i = 0; i <= RDF_NS_LAST; i++) {
    const unsigned char *value = attributes_element->rdf_attr[i];
    size_t value_len;
    int object_is_literal;
    raptor_uri *property_uri;
    raptor_term* object_term;
      
    if(!value)
      continue;

    value_len = strlen((const char*)value);
    
    object_is_literal = (raptor_rdf_ns_terms_info[i].type == RAPTOR_TERM_TYPE_LITERAL);

    if(raptor_rdf_ns_terms_info[i].type == RAPTOR_TERM_TYPE_UNKNOWN) {
      const char *name = raptor_rdf_ns_terms_info[i].name;
      int rc = raptor_rdfxml_check_propertyAttribute_name(name);
      if(!rc) {
        raptor_rdfxml_update_document_locator(rdf_parser);
        raptor_parser_error(rdf_parser,
                            "RDF term %s is forbidden as a property attribute.",
                            name);
        continue;
      } else if(rc < 0)
        raptor_parser_warning(rdf_parser,
                              "Unknown RDF namespace property attribute '%s'.", 
                              name);
    }

    if(object_is_literal &&
       !raptor_unicode_check_utf8_nfc_string(value, value_len, NULL)) {
      const char *message;
      message = "Property attribute '%s' has a string not in Unicode Normal Form C: %s";
      raptor_rdfxml_update_document_locator(rdf_parser);
      if(RAPTOR_OPTIONS_GET_NUMERIC(rdf_parser, RAPTOR_OPTION_NON_NFC_FATAL))
        raptor_parser_error(rdf_parser, message,
                            raptor_rdf_ns_terms_info[i].name, value);
      else
        raptor_parser_warning(rdf_parser, message,
                              raptor_rdf_ns_terms_info[i].name, value);
      continue;
    }

    property_uri = raptor_new_uri_for_rdf_concept(rdf_parser->world, 
                                                  (const unsigned char*)raptor_rdf_ns_terms_info[i].name);
    
    if(object_is_literal) {
      object_term = raptor_new_term_from_literal(rdf_parser->world,
                                                 (unsigned char*)value,
                                                 NULL, NULL);
    } else {
      raptor_uri *base_uri;
      raptor_uri *object_uri;
      base_uri = raptor_rdfxml_inscope_base_uri(rdf_parser);
      object_uri = raptor_new_uri_relative_to_base(rdf_parser->world, 
                                                   base_uri, value);
      object_term = raptor_new_term_from_uri(rdf_parser->world, object_uri);
      raptor_free_uri(object_uri);
    }
    
    raptor_rdfxml_generate_statement(rdf_parser,
                                     resource_identifier,
                                     property_uri,
                                     object_term,
                                     NULL,  
                                     resource_element);

    raptor_free_term(object_term);

    raptor_free_uri(property_uri);
    
  }  
  
  return 0;
}