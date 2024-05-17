raptor_rdfxml_start_element_grammar(raptor_parser *rdf_parser,
                                    raptor_rdfxml_element *element) 
{
  raptor_rdfxml_parser *rdf_xml_parser;
  int finished;
  raptor_state state;
  raptor_xml_element* xml_element;
  raptor_qname* el_qname;
  const unsigned char *el_name;
  int element_in_rdf_ns;
  int rc = 0;
  raptor_uri* base_uri;
  raptor_uri* element_name_uri;

  rdf_xml_parser = (raptor_rdfxml_parser*)rdf_parser->context;

  xml_element = element->xml_element;
  el_qname = raptor_xml_element_get_name(xml_element);
  el_name = el_qname->local_name;
  element_in_rdf_ns = (el_qname->nspace && el_qname->nspace->is_rdf_ms);
  base_uri = raptor_rdfxml_inscope_base_uri(rdf_parser);
  element_name_uri = el_qname->uri;
  
  state = element->state;
#ifdef RAPTOR_DEBUG_VERBOSE
  RAPTOR_DEBUG2("Starting in state %s\n", raptor_rdfxml_state_as_string(state));
#endif

  finished = 0;
  while(!finished) {

    switch(state) {
      case RAPTOR_STATE_SKIPPING:
        element->child_state = state;
        element->child_content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PRESERVED;
        finished = 1;
        break;
        
      case RAPTOR_STATE_UNKNOWN:
         

        if(element_in_rdf_ns) {
          if(raptor_uri_equals(element_name_uri,
                               RAPTOR_RDF_RDF_URI(rdf_parser->world))) {
            element->child_state = RAPTOR_STATE_NODE_ELEMENT_LIST;
            element->child_content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_NODES;
             
            finished = 1;
            break;
          }
          if(raptor_uri_equals(element_name_uri,
                               RAPTOR_RDF_Description_URI(rdf_parser->world))) {
            state = RAPTOR_STATE_DESCRIPTION;
            element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PROPERTIES;
             
            break;
          }

          if(element_in_rdf_ns) {
            rc = raptor_rdfxml_check_nodeElement_name((const char*)el_name);
            if(!rc) {
              raptor_parser_error(rdf_parser,
                                  "rdf:%s is forbidden as a node element.",
                                  el_name);
              state = RAPTOR_STATE_SKIPPING;
              element->child_state = RAPTOR_STATE_SKIPPING;
              finished = 1;
              break;
            } else if(rc < 0) {
              raptor_parser_warning(rdf_parser, 
                                    "rdf:%s is an unknown RDF namespaced element.", 
                                    el_name);
            }
          }
        }

         
        if(RAPTOR_OPTIONS_GET_NUMERIC(rdf_parser, RAPTOR_OPTION_SCANNING)) {
          finished = 1;
          break;
        }

         
        state = RAPTOR_STATE_NODE_ELEMENT_LIST;
        element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_NODES;
        break;


      case RAPTOR_STATE_NODE_ELEMENT_LIST:
         

        state = RAPTOR_STATE_NODE_ELEMENT;

        element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PROPERTIES;

        break;



      case RAPTOR_STATE_DESCRIPTION:
      case RAPTOR_STATE_NODE_ELEMENT:
      case RAPTOR_STATE_PARSETYPE_RESOURCE:
      case RAPTOR_STATE_PARSETYPE_COLLECTION:
         

        if(!element_name_uri) {
           
          raptor_parser_warning(rdf_parser, "Using node element '%s' without a namespace is forbidden.", 
                                el_qname->local_name);
          raptor_rdfxml_update_document_locator(rdf_parser);
          element->state = RAPTOR_STATE_SKIPPING;
          element->child_state = RAPTOR_STATE_SKIPPING;
          finished = 1;
          break;
        }

        if(element_in_rdf_ns) {
          rc = raptor_rdfxml_check_nodeElement_name((const char*)el_name);
          if(!rc) {
            raptor_parser_error(rdf_parser,
                                "rdf:%s is forbidden as a node element.",
                                el_name);
            state = RAPTOR_STATE_SKIPPING;
            element->state = RAPTOR_STATE_SKIPPING;
            element->child_state = RAPTOR_STATE_SKIPPING;
            finished = 1;
            break;
          } else if(rc < 0) {
            raptor_parser_warning(rdf_parser,
                                  "rdf:%s is an unknown RDF namespaced element.", 
                                  el_name);
          }
        }

        if(element->content_type != RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_COLLECTION &&
           element->content_type != RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_DAML_COLLECTION && 
           element->parent && 
           (element->parent->state == RAPTOR_STATE_PROPERTYELT ||
            element->parent->state == RAPTOR_STATE_MEMBER_PROPERTYELT) &&
           element->parent->xml_element->content_element_seen > 1) {
          raptor_rdfxml_update_document_locator(rdf_parser);
          raptor_parser_error(rdf_parser, "The enclosing property already has an object");
          state = RAPTOR_STATE_SKIPPING;
          element->child_state = RAPTOR_STATE_SKIPPING;
          finished = 1;
          break;
        }

        if(state == RAPTOR_STATE_NODE_ELEMENT || 
           state == RAPTOR_STATE_DESCRIPTION || 
           state == RAPTOR_STATE_PARSETYPE_COLLECTION) {
          if(element_in_rdf_ns &&
             raptor_uri_equals(element_name_uri,
                               RAPTOR_RDF_Description_URI(rdf_parser->world)))
            state = RAPTOR_STATE_DESCRIPTION;
          else
            state = RAPTOR_STATE_NODE_ELEMENT;
        }
        

        if((element->rdf_attr[RDF_NS_ID]!=NULL) +
           (element->rdf_attr[RDF_NS_about]!=NULL) +
           (element->rdf_attr[RDF_NS_nodeID]!=NULL) > 1) {
          raptor_rdfxml_update_document_locator(rdf_parser);
          raptor_parser_error(rdf_parser, "Multiple attributes of rdf:ID, rdf:about and rdf:nodeID on element '%s' - only one allowed.", el_name);
        }

        if(element->rdf_attr[RDF_NS_ID]) {
          unsigned char* subject_id;
          raptor_uri* subject_uri;
          
          subject_id = (unsigned char*)element->rdf_attr[RDF_NS_ID];

          if(!raptor_valid_xml_ID(rdf_parser, subject_id)) {
            raptor_parser_error(rdf_parser, "Illegal rdf:ID value '%s'", 
                                subject_id);
            state = RAPTOR_STATE_SKIPPING;
            element->child_state = RAPTOR_STATE_SKIPPING;
            finished = 1;
            break;
          }
          if(raptor_rdfxml_record_ID(rdf_parser, element, subject_id)) {
            raptor_parser_error(rdf_parser, "Duplicated rdf:ID value '%s'",
                                subject_id);
            state = RAPTOR_STATE_SKIPPING;
            element->child_state = RAPTOR_STATE_SKIPPING;
            finished = 1;
            break;
          }

           
          element->rdf_attr[RDF_NS_ID] = NULL;

          subject_uri = raptor_new_uri_from_id(rdf_parser->world, base_uri,
                                               subject_id);
          RAPTOR_FREE(char*, subject_id);

          if(!subject_uri)
            goto oom;
          element->subject = raptor_new_term_from_uri(rdf_parser->world,
                                                      subject_uri);
          raptor_free_uri(subject_uri);

          if(!element->subject)
            goto oom;

        } else if(element->rdf_attr[RDF_NS_about]) {
          raptor_uri* subject_uri;

          subject_uri = raptor_new_uri_relative_to_base(rdf_parser->world,
                                                        base_uri,
                                                        (const unsigned char*)element->rdf_attr[RDF_NS_about]);
          if(!subject_uri)
            goto oom;
          
          element->subject = raptor_new_term_from_uri(rdf_parser->world,
                                                      subject_uri);
          raptor_free_uri(subject_uri);

          RAPTOR_FREE(char*, element->rdf_attr[RDF_NS_about]);
          element->rdf_attr[RDF_NS_about] = NULL;
          if(!element->subject)
            goto oom;

        } else if(element->rdf_attr[RDF_NS_nodeID]) {
          unsigned char* subject_id;
          subject_id = raptor_world_internal_generate_id(rdf_parser->world,
                                                         (unsigned char*)element->rdf_attr[RDF_NS_nodeID]);
          if(!subject_id)
            goto oom;
          
          element->subject = raptor_new_term_from_blank(rdf_parser->world,
                                                        subject_id);
          RAPTOR_FREE(char*, subject_id);

          element->rdf_attr[RDF_NS_nodeID] = NULL;
          if(!element->subject)
            goto oom;

          if(!raptor_valid_xml_ID(rdf_parser, element->subject->value.blank.string)) {
            raptor_parser_error(rdf_parser, "Illegal rdf:nodeID value '%s'", 
                                (const char*)element->subject->value.blank.string);
            state = RAPTOR_STATE_SKIPPING;
            element->child_state = RAPTOR_STATE_SKIPPING;
            finished = 1;
            break;
          }
        } else if(element->parent && 
                   element->parent->child_content_type != RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_COLLECTION &&
                   element->parent->child_content_type != RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_DAML_COLLECTION &&
                   element->parent->object) {
           
          element->subject = raptor_term_copy(element->parent->object);
        } else {
          unsigned char* subject_id;
          subject_id = raptor_world_generate_bnodeid(rdf_parser->world);
          if(!subject_id)
            goto oom;

          element->subject = raptor_new_term_from_blank(rdf_parser->world,
                                                        subject_id);
          RAPTOR_FREE(char*, subject_id);

          if(!element->subject)
            goto oom;
        }


        if(element->rdf_attr[RDF_NS_bagID]) {
          if(RAPTOR_OPTIONS_GET_NUMERIC(rdf_parser, RAPTOR_OPTION_ALLOW_BAGID)) {
            unsigned char* bag_id;
            raptor_uri* bag_uri = NULL;

            bag_id = (unsigned char*)element->rdf_attr[RDF_NS_bagID];
            element->rdf_attr[RDF_NS_bagID] = NULL;

            bag_uri = raptor_new_uri_from_id(rdf_parser->world, 
                                             base_uri, bag_id);
            if(!bag_uri) {
              RAPTOR_FREE(char*, bag_id);
              goto oom;
            }

            element->bag = raptor_new_term_from_uri(rdf_parser->world, bag_uri);
            raptor_free_uri(bag_uri);

            if(!raptor_valid_xml_ID(rdf_parser, bag_id)) {
              raptor_parser_error(rdf_parser, "Illegal rdf:bagID value '%s'", 
                                  bag_id);
              state = RAPTOR_STATE_SKIPPING;
              element->child_state = RAPTOR_STATE_SKIPPING;
              finished = 1;
              RAPTOR_FREE(char*, bag_id);
              break;
            }
            if(raptor_rdfxml_record_ID(rdf_parser, element, bag_id)) {
              raptor_parser_error(rdf_parser, "Duplicated rdf:bagID value '%s'",
                                  bag_id);
              state = RAPTOR_STATE_SKIPPING;
              element->child_state = RAPTOR_STATE_SKIPPING;
              finished = 1;
              RAPTOR_FREE(char*, bag_id);
              break;
            }

            RAPTOR_FREE(char*, bag_id);
            raptor_parser_warning(rdf_parser, "rdf:bagID is deprecated.");

            
            raptor_rdfxml_generate_statement(rdf_parser, 
                                             element->bag,
                                             RAPTOR_RDF_type_URI(rdf_parser->world),
                                             RAPTOR_RDF_Bag_term(rdf_parser->world),
                                             NULL,
                                             NULL);
          } else {
             
            raptor_parser_error(rdf_parser, "rdf:bagID is forbidden.");
            state = RAPTOR_STATE_SKIPPING;
            element->child_state = RAPTOR_STATE_SKIPPING;
            finished = 1;
            break;
          }
        }


        if(element->parent) {

           
          if(element->content_type == RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_COLLECTION ||
             element->content_type == RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_DAML_COLLECTION) {
             
            const unsigned char * idList;
            raptor_uri *predicate_uri;
            raptor_term* idList_term;
            raptor_term* object_term;
              
            idList = raptor_world_generate_bnodeid(rdf_parser->world);
            if(!idList)
              goto oom;
             

            idList_term = raptor_new_term_from_blank(rdf_parser->world, idList);
            if(!idList_term) {
              RAPTOR_FREE(char*, idList);
              goto oom;
            }

            if((element->content_type == RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_DAML_COLLECTION) ||
               RAPTOR_OPTIONS_GET_NUMERIC(rdf_parser, RAPTOR_OPTION_ALLOW_RDF_TYPE_RDF_LIST)) {
              raptor_uri* class_uri = NULL;

              if(element->content_type == RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_DAML_COLLECTION) {
                class_uri = RAPTOR_DAML_List_URI(rdf_xml_parser);
                object_term = raptor_new_term_from_uri(rdf_parser->world,
                                                       class_uri);
              } else
                object_term = raptor_term_copy(RAPTOR_RDF_List_term(rdf_parser->world));
              
              raptor_rdfxml_generate_statement(rdf_parser, 
                                               idList_term,
                                               RAPTOR_RDF_type_URI(rdf_parser->world),
                                               object_term,
                                               NULL,
                                               element);
              raptor_free_term(object_term);
            }

            predicate_uri = (element->content_type == RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_DAML_COLLECTION) ? RAPTOR_DAML_first_URI(rdf_xml_parser) : RAPTOR_RDF_first_URI(rdf_parser->world);

             
            raptor_rdfxml_generate_statement(rdf_parser, 
                                             idList_term,
                                             predicate_uri,
                                             element->subject,
                                             NULL,
                                             NULL);
            
             
            if(!element->parent->tail_id) {
               
              if(element->parent->object)
                raptor_free_term(element->parent->object);

              element->parent->object = raptor_new_term_from_blank(rdf_parser->world,
                                                                   idList);
            } else {
              raptor_term* tail_id_term;

              tail_id_term = raptor_new_term_from_blank(rdf_parser->world, 
                                                        element->parent->tail_id);
              
              predicate_uri = (element->content_type == RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_DAML_COLLECTION) ? RAPTOR_DAML_rest_URI(rdf_xml_parser) : RAPTOR_RDF_rest_URI(rdf_parser->world);

               
              raptor_rdfxml_generate_statement(rdf_parser, 
                                               tail_id_term,
                                               predicate_uri,
                                               idList_term,
                                               NULL,
                                               NULL);

              raptor_free_term(tail_id_term);
            }

             
            if(element->parent->tail_id)
              RAPTOR_FREE(char*, (char*)element->parent->tail_id);

            element->parent->tail_id = idList;
            
            raptor_free_term(idList_term);
          } else if(element->parent->state != RAPTOR_STATE_UNKNOWN &&
                    element->state != RAPTOR_STATE_PARSETYPE_RESOURCE) {
             
            
            if(element->parent->object) {
              raptor_rdfxml_update_document_locator(rdf_parser);
              raptor_parser_error(rdf_parser,
                                  "Tried to set multiple objects of a statement");
            } else {
               
              element->parent->object = raptor_term_copy(element->subject);
              element->parent->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_RESOURCE;
            }

          }
        }
        

         
        if(state == RAPTOR_STATE_NODE_ELEMENT) {
          raptor_term* el_name_term;
              
          el_name_term = raptor_new_term_from_uri(rdf_parser->world,
                                                  element_name_uri);

          raptor_rdfxml_generate_statement(rdf_parser, 
                                           element->subject,
                                           RAPTOR_RDF_type_URI(rdf_parser->world),
                                           el_name_term,
                                           element->reified,
                                           element);

          raptor_free_term(el_name_term);
        }

        if(raptor_rdfxml_process_property_attributes(rdf_parser, element,
                                                     element, NULL))
          goto oom;

         

        element->child_state = RAPTOR_STATE_PROPERTYELT;
        element->child_content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PROPERTIES;
        finished = 1;
        break;


      case RAPTOR_STATE_PARSETYPE_OTHER:
         

      case RAPTOR_STATE_PARSETYPE_LITERAL:
        raptor_xml_writer_start_element(rdf_xml_parser->xml_writer, xml_element);
        element->child_state = RAPTOR_STATE_PARSETYPE_LITERAL;
        element->child_content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_XML_LITERAL;
        
        finished = 1;
        break;

         
      case RAPTOR_STATE_MEMBER_PROPERTYELT:
      case RAPTOR_STATE_PROPERTYELT:

        if(!element_name_uri) {
          raptor_parser_error(rdf_parser, "Using property element '%s' without a namespace is forbidden.", 
                              raptor_xml_element_get_name(element->parent->xml_element)->local_name);
          raptor_rdfxml_update_document_locator(rdf_parser);
          element->state = RAPTOR_STATE_SKIPPING;
          element->child_state = RAPTOR_STATE_SKIPPING;
          finished = 1;
          break;
        }

          
        if(element_in_rdf_ns && 
           raptor_uri_equals(element_name_uri,
                             RAPTOR_RDF_li_URI(rdf_parser->world))) {
          state = RAPTOR_STATE_MEMBER_PROPERTYELT;
        }


        if(element_in_rdf_ns) {
          rc = raptor_rdfxml_check_propertyElement_name((const char*)el_name);
          if(!rc) {
            raptor_parser_error(rdf_parser, 
                                "rdf:%s is forbidden as a property element.",
                                el_name);
            state = RAPTOR_STATE_SKIPPING;
            element->child_state = RAPTOR_STATE_SKIPPING;
            finished = 1;
            break;
          } else if(rc < 0) {
            raptor_parser_warning(rdf_parser,
                                  "rdf:%s is an unknown RDF namespaced element.",
                                  el_name);
          }
        }
          

         
        if(element->rdf_attr[RDF_NS_ID]) {
          raptor_uri *reified_uri;
          
          element->reified_id = element->rdf_attr[RDF_NS_ID];
          element->rdf_attr[RDF_NS_ID] = NULL;
          reified_uri = raptor_new_uri_from_id(rdf_parser->world, base_uri,
                                               element->reified_id);
          if(!reified_uri)
            goto oom;

          element->reified = raptor_new_term_from_uri(rdf_parser->world,
                                                      reified_uri);
          raptor_free_uri(reified_uri);

          if(!element->reified)
            goto oom;

          if(!raptor_valid_xml_ID(rdf_parser, element->reified_id)) {
            raptor_parser_error(rdf_parser, "Illegal rdf:ID value '%s'",
                                element->reified_id);
            state = RAPTOR_STATE_SKIPPING;
            element->child_state = RAPTOR_STATE_SKIPPING;
            finished = 1;
            break;
          }
          if(raptor_rdfxml_record_ID(rdf_parser, element, element->reified_id)) {
            raptor_parser_error(rdf_parser, "Duplicated rdf:ID value '%s'",
                                element->reified_id);
            state = RAPTOR_STATE_SKIPPING;
            element->child_state = RAPTOR_STATE_SKIPPING;
            finished = 1;
            break;
          }
        }
        
         
        if(element->rdf_attr[RDF_NS_datatype]) {
          raptor_uri *datatype_uri;
          
          datatype_uri = raptor_new_uri_relative_to_base(rdf_parser->world, 
                                                         base_uri,
                                                         (const unsigned char*)element->rdf_attr[RDF_NS_datatype]);
          element->object_literal_datatype = datatype_uri;
          RAPTOR_FREE(char*, element->rdf_attr[RDF_NS_datatype]);
          element->rdf_attr[RDF_NS_datatype] = NULL; 
          if(!element->object_literal_datatype)
            goto oom;
        }

        if(element->rdf_attr[RDF_NS_bagID]) {

          if(RAPTOR_OPTIONS_GET_NUMERIC(rdf_parser, RAPTOR_OPTION_ALLOW_BAGID)) {

            if(element->rdf_attr[RDF_NS_resource] ||
               element->rdf_attr[RDF_NS_parseType]) {
              
              raptor_parser_error(rdf_parser, "rdf:bagID is forbidden on property element '%s' with an rdf:resource or rdf:parseType attribute.", el_name);
               
              element->rdf_attr[RDF_NS_bagID] = NULL;
            } else {
              unsigned char* bag_id;
              raptor_uri* bag_uri;
              
              bag_id = (unsigned char*)element->rdf_attr[RDF_NS_bagID];
              element->rdf_attr[RDF_NS_bagID] = NULL;
              bag_uri = raptor_new_uri_from_id(rdf_parser->world, base_uri,
                                               bag_id);
              if(!bag_uri) {
                RAPTOR_FREE(char*, bag_id);
                goto oom;
              }

              element->bag = raptor_new_term_from_uri(rdf_parser->world,
                                                      bag_uri);
              raptor_free_uri(bag_uri);

              if(!element->bag) {
                RAPTOR_FREE(char*, bag_id);
                goto oom;
              }
              
              if(!raptor_valid_xml_ID(rdf_parser, bag_id)) {
                raptor_parser_error(rdf_parser, "Illegal rdf:bagID value '%s'",
                                    bag_id);
                state = RAPTOR_STATE_SKIPPING;
                element->child_state = RAPTOR_STATE_SKIPPING;
                finished = 1;
                RAPTOR_FREE(char*, bag_id);
                break;
              }
              if(raptor_rdfxml_record_ID(rdf_parser, element, bag_id)) {
                raptor_parser_error(rdf_parser,
                                    "Duplicated rdf:bagID value '%s'", bag_id);
                state = RAPTOR_STATE_SKIPPING;
                element->child_state = RAPTOR_STATE_SKIPPING;
                RAPTOR_FREE(char*, bag_id);
                finished = 1;
                break;
              }

              RAPTOR_FREE(char*, bag_id);
              raptor_parser_warning(rdf_parser, "rdf:bagID is deprecated.");
            }
          } else {
             
            raptor_parser_error(rdf_parser, "rdf:bagID is forbidden.");
            state = RAPTOR_STATE_SKIPPING;
            element->child_state = RAPTOR_STATE_SKIPPING;
            finished = 1;
            break;
          }
        }  
        

        element->child_content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PROPERTY_CONTENT;

        if(element->rdf_attr[RDF_NS_parseType]) {
          const unsigned char *parse_type;
          int i;
          int is_parseType_Literal = 0;

          parse_type = element->rdf_attr[RDF_NS_parseType];

          if(raptor_rdfxml_element_has_property_attributes(element)) {
            raptor_parser_error(rdf_parser, "Property attributes cannot be used with rdf:parseType='%s'", parse_type);
            state = RAPTOR_STATE_SKIPPING;
            element->child_state = RAPTOR_STATE_SKIPPING;
            finished = 1;
            break;
          }

           
          for(i = 0; i <= RDF_NS_LAST; i++)
            if(element->rdf_attr[i] && i != RDF_NS_parseType) {
              raptor_parser_error(rdf_parser, "Attribute '%s' cannot be used with rdf:parseType='%s'", raptor_rdf_ns_terms_info[i].name, parse_type);
              state = RAPTOR_STATE_SKIPPING;
              element->child_state = RAPTOR_STATE_SKIPPING;
              break;
            }


          if(!strcmp((char*)parse_type, "Literal"))
            is_parseType_Literal = 1;
          else if(!strcmp((char*)parse_type, "Resource")) {
            unsigned char* subject_id;
            
            state = RAPTOR_STATE_PARSETYPE_RESOURCE;
            element->child_state = RAPTOR_STATE_PROPERTYELT;
            element->child_content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PROPERTIES;

             
            subject_id = raptor_world_generate_bnodeid(rdf_parser->world);
            if(!subject_id)
              goto oom;

            element->subject = raptor_new_term_from_blank(rdf_parser->world,
                                                          subject_id);
            RAPTOR_FREE(char*, subject_id);

            if(!element->subject)
              goto oom;
          } else if(!strcmp((char*)parse_type, "Collection")) {
             
            element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_RESOURCE;
            element->child_state = RAPTOR_STATE_PARSETYPE_COLLECTION;
            element->child_content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_COLLECTION;
          } else {
            if(RAPTOR_OPTIONS_GET_NUMERIC(rdf_parser, RAPTOR_OPTION_ALLOW_OTHER_PARSETYPES) &&
               !raptor_strcasecmp((char*)parse_type, "daml:collection")) {
                 
                element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_RESOURCE;
                element->child_state = RAPTOR_STATE_PARSETYPE_COLLECTION;
                element->child_content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_DAML_COLLECTION;
            } else {
              if(RAPTOR_OPTIONS_GET_NUMERIC(rdf_parser, RAPTOR_OPTION_WARN_OTHER_PARSETYPES)) {
                raptor_parser_warning(rdf_parser, "Unknown rdf:parseType value '%s' taken as 'Literal'", parse_type);
              }
              is_parseType_Literal = 1;
            }
            
          }
          
          if(is_parseType_Literal) {
            raptor_xml_writer* xml_writer;

             
            rdf_xml_parser->xml_content = NULL;
            rdf_xml_parser->xml_content_length = 0;
            rdf_xml_parser->iostream =
              raptor_new_iostream_to_string(rdf_parser->world,
                                            &rdf_xml_parser->xml_content,
                                            &rdf_xml_parser->xml_content_length,
                                            raptor_alloc_memory);
            if(!rdf_xml_parser->iostream)
              goto oom;
            xml_writer = raptor_new_xml_writer(rdf_parser->world, NULL,
                                               rdf_xml_parser->iostream);
            rdf_xml_parser->xml_writer = xml_writer;
            if(!rdf_xml_parser->xml_writer)
              goto oom;
            
            raptor_xml_writer_set_option(rdf_xml_parser->xml_writer, 
                                         RAPTOR_OPTION_WRITER_XML_DECLARATION,
                                         NULL, 0);

            element->child_state = RAPTOR_STATE_PARSETYPE_LITERAL;
            element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_XML_LITERAL;
            element->child_content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_XML_LITERAL;
          }
        } else {

           

           

           
          if(element->reified_id && !element->reified) {
            raptor_uri* reified_uri;
            reified_uri = raptor_new_uri_from_id(rdf_parser->world, base_uri,
                                                 element->reified_id);
            if(!reified_uri)
              goto oom;
            element->reified = raptor_new_term_from_uri(rdf_parser->world,
                                                        reified_uri);
            raptor_free_uri(reified_uri);

            if(!element->reified)
              goto oom;
          }

          if(element->rdf_attr[RDF_NS_resource] ||
             element->rdf_attr[RDF_NS_nodeID]) {
             
            element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_RESOURCE;
          } else {
             
            element->child_state = RAPTOR_STATE_NODE_ELEMENT_LIST;
            element->content_type = RAPTOR_RDFXML_ELEMENT_CONTENT_TYPE_PROPERTY_CONTENT;
          }
        }

        finished = 1;

        break;


      case RAPTOR_STATE_INVALID:
      default:
        raptor_parser_fatal_error(rdf_parser,
                                  "%s Internal error - unexpected parser state %d - %s",
                                  __func__,
                                  state, raptor_rdfxml_state_as_string(state));
        finished = 1;

    }  

    if(state != element->state) {
      element->state = state;
#ifdef RAPTOR_DEBUG_VERBOSE
      RAPTOR_DEBUG3("Moved to state %d - %s\n", state,
                    raptor_rdfxml_state_as_string(state));
#endif
    }

  }  

#ifdef RAPTOR_DEBUG_VERBOSE
  RAPTOR_DEBUG2("Ending in state %s\n", raptor_rdfxml_state_as_string(state));
#endif

  return;

  oom:
  raptor_parser_fatal_error(rdf_parser, "Out of memory, skipping");
  element->state = RAPTOR_STATE_SKIPPING;
}