raptor_rdfxml_start_element_handler(void *user_data,
                                    raptor_xml_element* xml_element)
{
  raptor_parser* rdf_parser;
  raptor_rdfxml_parser* rdf_xml_parser;
  raptor_rdfxml_element* element;
  int ns_attributes_count = 0;
  raptor_qname** named_attrs = NULL;
  int i;
  int count_bumped = 0;
  
  rdf_parser = (raptor_parser*)user_data;
  rdf_xml_parser = (raptor_rdfxml_parser*)rdf_parser->context;
  
  if(rdf_parser->failed)
    return;

  raptor_rdfxml_update_document_locator(rdf_parser);

   
  element = RAPTOR_CALLOC(raptor_rdfxml_element*, 1, sizeof(*element));
  if(!element) {
    raptor_parser_fatal_error(rdf_parser, "Out of memory");
    rdf_parser->failed = 1;
    return;
  }
  element->world = rdf_parser->world;
  element->xml_element = xml_element;

  raptor_rdfxml_element_push(rdf_xml_parser, element);

  named_attrs = raptor_xml_element_get_attributes(xml_element);
  ns_attributes_count = raptor_xml_element_get_attributes_count(xml_element);

   
  if(ns_attributes_count) {
    raptor_qname** new_named_attrs;
    int offset = 0;
    raptor_rdfxml_element* parent_element;

    parent_element = element->parent;

     
    new_named_attrs = RAPTOR_CALLOC(raptor_qname**, ns_attributes_count, 
                                    sizeof(raptor_qname*));
    if(!new_named_attrs) {
      raptor_parser_fatal_error(rdf_parser, "Out of memory");
      rdf_parser->failed = 1;
      return;
    }

    for(i = 0; i < ns_attributes_count; i++) {
      raptor_qname* attr = named_attrs[i];

       
      if((parent_element &&
          rdf_content_type_info[parent_element->child_content_type].rdf_processing) ||
         !parent_element) {

         
        
         
        if(attr->nspace && attr->nspace->is_rdf_ms) {
          const unsigned char *attr_name = attr->local_name;
          int j;

          for(j = 0; j <= RDF_NS_LAST; j++)
            if(!strcmp((const char*)attr_name,
                       raptor_rdf_ns_terms_info[j].name)) {
              element->rdf_attr[j] = attr->value;
              element->rdf_attr_count++;
               
#ifdef RAPTOR_DEBUG_VERBOSE
              RAPTOR_DEBUG3("Found RDF namespace attribute '%s' URI %s\n",
                            (char*)attr_name, attr->value);
#endif
               
              attr->value = NULL;
              raptor_free_qname(attr);
              attr = NULL;
              break;
            }
        }  

        if(!attr)
          continue;

         
        if(RAPTOR_OPTIONS_GET_NUMERIC(rdf_parser, RAPTOR_OPTION_ALLOW_NON_NS_ATTRIBUTES) &&
           !attr->nspace) {
          const unsigned char *attr_name = attr->local_name;
          int j;

          for(j = 0; j <= RDF_NS_LAST; j++)
            if(!strcmp((const char*)attr_name,
                       raptor_rdf_ns_terms_info[j].name)) {
              element->rdf_attr[j] = attr->value;
              element->rdf_attr_count++;
              if(!raptor_rdf_ns_terms_info[j].allowed_unprefixed_on_attribute)
                raptor_parser_warning(rdf_parser,
                                      "Using rdf attribute '%s' without the RDF namespace has been deprecated.",
                                      attr_name);

               
               
              attr->value = NULL;
              raptor_free_qname(attr);
              attr = NULL;
              break;
            }
        }  

        if(!attr)
          continue;

      }  

      if(attr)
        new_named_attrs[offset++] = attr;
    }

     
    ns_attributes_count = offset;
    if(!ns_attributes_count) {
       
      RAPTOR_FREE(raptor_qname_array, new_named_attrs);
      new_named_attrs = NULL;
    }

    RAPTOR_FREE(raptor_qname_array, named_attrs);
    named_attrs = new_named_attrs;
    raptor_xml_element_set_attributes(xml_element, 
                                      named_attrs, ns_attributes_count);
  }  


   
  element->state = RAPTOR_STATE_UNKNOWN;
  element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_UNKNOWN;

  if(element->parent && 
     element->parent->child_content_type != RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_UNKNOWN) {
    element->content_type = element->parent->child_content_type;
      
    if(element->parent->content_type == RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_RESOURCE &&
       element->content_type != RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_COLLECTION &&
       element->content_type != RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_DAML_COLLECTION) {
      raptor_qname* parent_el_name;
      parent_el_name = raptor_xml_element_get_name(element->parent->xml_element);
       
      raptor_parser_error(rdf_parser,
                          "property element '%s' has multiple object node elements, skipping.", 
                          parent_el_name->local_name);
      element->state = RAPTOR_STATE_SKIPPING;
      element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PRESERVED;

    } else {
      if(!element->parent->child_state) {
        raptor_parser_fatal_error(rdf_parser,
                                  "%s: Internal error: no parent element child_state set",
                                  __func__);
        return;
      }

      element->state = element->parent->child_state;
      element->parent->xml_element->content_element_seen++;
      count_bumped++;
    
       
      if(!rdf_content_type_info[element->content_type].cdata_allowed) {
        if(element->parent->xml_element->content_element_seen &&
           element->parent->xml_element->content_cdata_seen) {
          raptor_qname* parent_el_name;

          parent_el_name = raptor_xml_element_get_name(element->parent->xml_element);
           
          raptor_parser_warning(rdf_parser, "element '%s' has mixed content.", 
                                parent_el_name->local_name);
        }
        
         
        if(element->parent->content_type == RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PROPERTIES &&
           element->parent->xml_element->content_element_seen &&
           element->parent->content_cdata_all_whitespace &&
           element->parent->xml_element->content_cdata_length) {
          
          element->parent->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_RESOURCE;
          
          raptor_free_stringbuffer(element->parent->xml_element->content_cdata_sb);
          element->parent->xml_element->content_cdata_sb = NULL;
          element->parent->xml_element->content_cdata_length = 0;
        }
        
      }  
      
    }  

  }  


#ifdef RAPTOR_DEBUG_VERBOSE
  RAPTOR_DEBUG2("Using content type %s\n",
                rdf_content_type_info[element->content_type].name);

  fprintf(stderr, "raptor_rdfxml_start_element_handler: Start ns-element: ");
  raptor_print_xml_element(xml_element, stderr);
#endif

  
   
  if(rdf_content_type_info[element->content_type].rdf_processing) {
    const raptor_namespace* ns;

    ns = raptor_xml_element_get_name(xml_element)->nspace;
     

     
    if((!ns || (ns && !raptor_namespace_get_uri(ns))) && element->parent) {
      raptor_qname* parent_el_name;

      parent_el_name = raptor_xml_element_get_name(element->parent->xml_element);

      raptor_parser_error(rdf_parser,
                          "Using an element '%s' without a namespace is forbidden.", 
                          parent_el_name->local_name);
      element->state = RAPTOR_STATE_SKIPPING;
       
      if(count_bumped)
        element->parent->xml_element->content_element_seen--;
      element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PRESERVED;
    }


     
    if(named_attrs) {
      for(i = 0; i < ns_attributes_count; i++) {
        raptor_qname *attr = named_attrs[i];
         
        if(!attr->nspace ||
           (attr->nspace && !raptor_namespace_get_uri(attr->nspace))) {
          raptor_parser_error(rdf_parser,
                              "Using an attribute '%s' without a namespace is forbidden.", 
                              attr->local_name);
          raptor_free_qname(attr);
          named_attrs[i] = NULL;
        }
      }
    }
  }
  

  if(element->rdf_attr[RDF_NS_aboutEach] || 
     element->rdf_attr[RDF_NS_aboutEachPrefix]) {
    raptor_parser_warning(rdf_parser,
                          "element '%s' has aboutEach / aboutEachPrefix, skipping.", 
                          raptor_xml_element_get_name(xml_element)->local_name);
    element->state = RAPTOR_STATE_SKIPPING;
     
    if(count_bumped)
      element->parent->xml_element->content_element_seen--;
    element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PRESERVED;
  }
  
   
  raptor_rdfxml_start_element_grammar(rdf_parser, element);

  return;
}
