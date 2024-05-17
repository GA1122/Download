raptor_rss_start_element_handler(void *user_data,
                                 raptor_xml_element* xml_element)
{
  raptor_parser *rdf_parser;
  raptor_rss_parser *rss_parser;
  raptor_rss_block *block = NULL;
  raptor_uri* base_uri;
  raptor_qname *el_qname;
  const unsigned char *name;
  int ns_attributes_count;
  raptor_qname** named_attrs;
  const raptor_namespace* el_nspace;
  raptor_rss_element* rss_element;
  int i;

  rdf_parser = (raptor_parser*)user_data;
  rss_parser = (raptor_rss_parser*)rdf_parser->context;

  rss_element = RAPTOR_CALLOC(raptor_rss_element*, 1, sizeof(*rss_element));
  if(!rss_element) {
    rdf_parser->failed = 1;
    return;
  }

  rss_element->world = rdf_parser->world;
  rss_element->sb = raptor_new_stringbuffer();

  xml_element->user_data = rss_element;

  if(xml_element->parent) {
    raptor_rss_element* parent_rss_element;
    parent_rss_element = (raptor_rss_element*)(xml_element->parent->user_data);
    if(parent_rss_element->xml_writer)
      rss_element->xml_writer = parent_rss_element->xml_writer;
  }

  if(rss_element->xml_writer) {
    raptor_xml_writer_start_element(rss_element->xml_writer, xml_element);
    return;
  }

  el_qname = raptor_xml_element_get_name(xml_element);
  name = el_qname->local_name;
  el_nspace = el_qname->nspace;

  named_attrs = raptor_xml_element_get_attributes(xml_element);
  ns_attributes_count = raptor_xml_element_get_attributes_count(xml_element);
  
  base_uri = raptor_sax2_inscope_base_uri(rss_parser->sax2);


   
  if(!raptor_rss_add_container(rss_parser, (const char*)name)) {
#ifdef RAPTOR_DEBUG
    if(1) {
      raptor_rss_type old_type = rss_parser->prev_type;

      if(old_type != rss_parser->current_type && old_type != RAPTOR_RSS_NONE)
        RAPTOR_DEBUG5("FOUND inner container type %d - %s INSIDE current container type %d - %s\n", rss_parser->current_type,
                      raptor_rss_items_info[rss_parser->current_type].name,
                      old_type, raptor_rss_items_info[old_type].name);
      else
        RAPTOR_DEBUG3("FOUND container type %d - %s\n",
                      rss_parser->current_type,
                      raptor_rss_items_info[rss_parser->current_type].name);
    }
#endif
    
     
    if(named_attrs) {
      raptor_rss_item* update_item = raptor_rss_get_current_item(rss_parser);

      for(i = 0; i < ns_attributes_count; i++) {
        raptor_qname* attr = named_attrs[i];
        const char* attrName = (const char*)attr->local_name;
        const unsigned char* attrValue = attr->value;

        RAPTOR_DEBUG3("  container attribute %s=%s\n", attrName, attrValue);
        if(!strcmp(attrName, "about")) {
          if(update_item) {
            update_item->uri = raptor_new_uri(rdf_parser->world, attrValue);
            update_item->term = raptor_new_term_from_uri(rdf_parser->world,
                                                         update_item->uri);
          }
        }
      }
    }
    return;
  } else if(rss_parser->current_type == RAPTOR_RSS_NONE) {
    RAPTOR_DEBUG2("Unknown container element named %s\n", name);
     
    return;
  }


   

   
  rss_parser->current_field = RAPTOR_RSS_FIELD_UNKNOWN;
  for(i = 0; i < RAPTOR_RSS_FIELDS_SIZE; i++) {
    raptor_uri* nspace_URI;
    raptor_uri* field_nspace_URI;
    rss_info_namespace nsid = raptor_rss_fields_info[i].nspace;
    
    if(strcmp((const char*)name, raptor_rss_fields_info[i].name))
      continue;
    
    if(!el_nspace) {
      if(nsid != RSS_NO_NS && nsid != RSS1_0_NS && nsid != RSS0_91_NS &&
         nsid != RSS0_9_NS && nsid != RSS1_1_NS)
        continue;

       
      rss_parser->current_field = (raptor_rss_fields_type)i;
      break;
    }
    
     
    nspace_URI = raptor_rss_promote_namespace_uri(rdf_parser->world,
                                                  raptor_namespace_get_uri(el_nspace));
    field_nspace_URI = rdf_parser->world->rss_namespaces_info_uris[raptor_rss_fields_info[i].nspace];
    
    if(raptor_uri_equals(nspace_URI,
                         field_nspace_URI)) {
      rss_parser->current_field = (raptor_rss_fields_type)i;
      break;
    }
  }

  if(rss_parser->current_field == RAPTOR_RSS_FIELD_UNKNOWN) {
    RAPTOR_DEBUG3("Unknown field element named %s inside type %s\n", name, 
                  raptor_rss_items_info[rss_parser->current_type].name);
    return;
  }


   
  if(raptor_rss_fields_info[rss_parser->current_field].flags & 
     RAPTOR_RSS_INFO_FLAG_BLOCK_VALUE) {
    raptor_rss_type block_type;
    raptor_rss_item* update_item;
    const unsigned char *id;
    raptor_term* block_term;
    
    block_type = raptor_rss_fields_info[rss_parser->current_field].block_type;

    RAPTOR_DEBUG3("FOUND new block type %d - %s\n", block_type,
                  raptor_rss_items_info[block_type].name);

    update_item = raptor_rss_get_current_item(rss_parser);

    id = raptor_world_generate_bnodeid(rdf_parser->world);
    block_term = raptor_new_term_from_blank(rdf_parser->world, id);
    RAPTOR_FREE(char*, id);

    block = raptor_new_rss_block(rdf_parser->world, block_type, block_term);
    raptor_free_term(block_term);

    raptor_rss_item_add_block(update_item, block);
    rss_parser->current_block = block;

    rss_parser->nspaces_seen[raptor_rss_items_info[block_type].nspace] = 'Y';
    
     
    if(named_attrs) {
      for(i = 0; i < ns_attributes_count; i++) {
        raptor_qname* attr = named_attrs[i];
        const char* attrName = (const char*)attr->local_name;
        const unsigned char* attrValue = attr->value;
        const raptor_rss_block_field_info *bfi;
        int offset = -1;
        
        for(bfi = &raptor_rss_block_fields_info[0];
            bfi->type != RAPTOR_RSS_NONE;
            bfi++) {
          if(!bfi->attribute)
            continue;
          
          if(bfi->type == block_type && !strcmp(attrName, bfi->attribute)) {
            offset = bfi->offset;
            break;
          }
        }

        if(offset < 0)
          continue;
        
         
        RAPTOR_DEBUG3("  found block attribute %s=%s\n", attrName, attrValue);
        if(raptor_rss_block_set_field(rdf_parser->world, base_uri,
                                      block, bfi, (const char*)attrValue)) {
          rdf_parser->failed = 1;
          return;
        }

      }

    }

    return;
  }


   
  RAPTOR_DEBUG4("FOUND field %d - %s inside type %s\n",
                rss_parser->current_field,
                raptor_rss_fields_info[rss_parser->current_field].name,
                raptor_rss_items_info[rss_parser->current_type].name);
  
   
  if(1) {
    rss_info_namespace ns_index;
    ns_index = raptor_rss_fields_info[rss_parser->current_field].nspace;
    rss_parser->nspaces_seen[ns_index] = 'Y';
  }
  
  
   
  if(named_attrs) {
    for(i = 0; i < ns_attributes_count; i++) {
      raptor_qname* attr = named_attrs[i];
      const unsigned char* attrName = attr->local_name;
      const unsigned char* attrValue = attr->value;

      RAPTOR_DEBUG3("  attribute %s=%s\n", attrName, attrValue);

       
      if(!strcmp((const char*)attrName, "isPermaLink")) {
        raptor_rss_item* update_item = rss_parser->model.last;
        if(!strcmp((const char*)name, "guid")) {
           
          if(update_item) {
            raptor_rss_field* field = raptor_rss_new_field(rdf_parser->world);
            RAPTOR_DEBUG1("fa1 - ");
            raptor_rss_item_add_field(update_item, RAPTOR_RSS_FIELD_GUID, field);
            if(!strcmp((const char*)attrValue, "true")) {
              RAPTOR_DEBUG2("    setting guid to URI '%s'\n", attrValue);
              field->uri = raptor_new_uri_relative_to_base(rdf_parser->world, base_uri,
                                                           (const unsigned char*)attrValue);
            } else {
              size_t len = strlen((const char*)attrValue);
              RAPTOR_DEBUG2("    setting guid to string '%s'\n", attrValue);
              field->value = RAPTOR_MALLOC(unsigned char*, len + 1);
              if(!field->value) {
                rdf_parser->failed = 1;
                return;
              }
              memcpy(field->value, attrValue, len + 1);
            }
          }
        }
      } else if(!strcmp((const char*)attrName, "href")) {
        if(rss_parser->current_field == RAPTOR_RSS_FIELD_LINK ||
           rss_parser->current_field == RAPTOR_RSS_FIELD_ATOM_LINK) {
          RAPTOR_DEBUG2("  setting href as URI string for type %s\n", raptor_rss_items_info[rss_parser->current_type].name);
          if(rss_element->uri)
            raptor_free_uri(rss_element->uri);
          rss_element->uri = raptor_new_uri_relative_to_base(rdf_parser->world, base_uri,
                                                             (const unsigned char*)attrValue);
        }
      } else if(!strcmp((const char*)attrName, "type")) {
        if(rss_parser->current_field == RAPTOR_RSS_FIELD_ATOM_LINK) {
           
        } else if(rss_parser->is_atom) {
           
          if(!strcmp((const char*)attrValue, "xhtml") ||
              !strcmp((const char*)attrValue, "xml") ||
              strstr((const char*)attrValue, "+xml")) {

            RAPTOR_DEBUG2("  found type '%s', making an XML writer\n", 
                          attrValue);
            
            rss_element->type = RAPTOR_RSS_CONTENT_TYPE_XML;
            rss_element->iostream = raptor_new_iostream_to_string(rdf_parser->world,
                                                                  &rss_element->xml_content,
                                                                  &rss_element->xml_content_length,
                                                                  raptor_alloc_memory);
            rss_element->xml_writer = raptor_new_xml_writer(rdf_parser->world,
                                                            NULL,
                                                            rss_element->iostream);
            raptor_xml_writer_set_option(rss_element->xml_writer, 
                                         RAPTOR_OPTION_WRITER_XML_DECLARATION,
                                         NULL, 0);

            raptor_free_stringbuffer(rss_element->sb);
            rss_element->sb = NULL;

          }
        }
      } else if(!strcmp((const char*)attrName, "version")) {
        if(!raptor_strcasecmp((const char*)name, "feed")) {
          if(!strcmp((const char*)attrValue, "0.3"))
            rss_parser->is_atom = 1;
        }
      }
    }
  }  

}