raptor_rss_insert_identifiers(raptor_parser* rdf_parser) 
{
  raptor_rss_parser* rss_parser = (raptor_rss_parser*)rdf_parser->context;
  int i;
  raptor_rss_item* item;
  
  for(i = 0; i< RAPTOR_RSS_COMMON_SIZE; i++) {
    for(item = rss_parser->model.common[i]; item; item = item->next) {
      if(!item->fields_count)
        continue;
      
      RAPTOR_DEBUG3("Inserting identifiers in common type %d - %s\n", i, raptor_rss_items_info[i].name);
    
      if(item->uri) {
        item->term = raptor_new_term_from_uri(rdf_parser->world, item->uri);
      } else {
        int url_fields[2];
        int url_fields_count = 1;
        int f;
      
        url_fields[0] = (i== RAPTOR_RSS_IMAGE) ? RAPTOR_RSS_FIELD_URL :
                                                 RAPTOR_RSS_FIELD_LINK;
        if(i == RAPTOR_RSS_CHANNEL) {
          url_fields[1] = RAPTOR_RSS_FIELD_ATOM_ID;
          url_fields_count++;
        }

        for(f = 0; f < url_fields_count; f++) {
          raptor_rss_field* field;

          for(field = item->fields[url_fields[f]]; field; field = field->next) {
            raptor_uri *new_uri = NULL;
            if(field->value)
              new_uri = raptor_new_uri(rdf_parser->world,
                                       (const unsigned char*)field->value);
            else if(field->uri)
              new_uri = raptor_uri_copy(field->uri);

            if(!new_uri)
              return 1;
            
            item->term = raptor_new_term_from_uri(rdf_parser->world, new_uri);
            raptor_free_uri(new_uri);
            if(!item->term)
              return 1;

            break;
          }
        }
      
        if(!item->term) {
          const unsigned char *id;

           
          id = raptor_world_generate_bnodeid(rdf_parser->world);
          item->term = raptor_new_term_from_blank(rdf_parser->world, id);
          RAPTOR_FREE(char*, id);
        }
      }

       
      if(i == RAPTOR_RSS_CHANNEL && !item->fields[RAPTOR_RSS_FIELD_LINK]) {
        if(raptor_rss_insert_rss_link(rdf_parser, item))
          return 1;
      }

      item->node_type = &raptor_rss_items_info[i];
      item->node_typei = i;
    }
  }
   
  for(item = rss_parser->model.items; item; item = item->next) {
    raptor_rss_block *block;
    raptor_uri* uri;
    
    if(!item->fields[RAPTOR_RSS_FIELD_LINK])  {
      if(raptor_rss_insert_rss_link(rdf_parser, item))
        return 1;
    }
    

    if(item->uri) {
      uri = raptor_uri_copy(item->uri);
    } else {
      if(item->fields[RAPTOR_RSS_FIELD_LINK]) {
        if(item->fields[RAPTOR_RSS_FIELD_LINK]->value)
          uri = raptor_new_uri(rdf_parser->world,
                               (const unsigned char*)item->fields[RAPTOR_RSS_FIELD_LINK]->value);
        else if(item->fields[RAPTOR_RSS_FIELD_LINK]->uri)
          uri = raptor_uri_copy(item->fields[RAPTOR_RSS_FIELD_LINK]->uri);
      } else if(item->fields[RAPTOR_RSS_FIELD_ATOM_ID]) {
        if(item->fields[RAPTOR_RSS_FIELD_ATOM_ID]->value)
          uri = raptor_new_uri(rdf_parser->world,
                               (const unsigned char*)item->fields[RAPTOR_RSS_FIELD_ATOM_ID]->value);
        else if(item->fields[RAPTOR_RSS_FIELD_ATOM_ID]->uri)
          uri = raptor_uri_copy(item->fields[RAPTOR_RSS_FIELD_ATOM_ID]->uri);
      }
    }

    item->term = raptor_new_term_from_uri(rdf_parser->world, uri);
    raptor_free_uri(uri);
    uri = NULL;
    
    for(block = item->blocks; block; block = block->next) {
      if(!block->identifier) {
        const unsigned char *id;
         
        id = raptor_world_generate_bnodeid(rdf_parser->world);
        item->term = raptor_new_term_from_blank(rdf_parser->world, id);
        RAPTOR_FREE(char*, id);
      }
    }
    
    item->node_type = &raptor_rss_items_info[RAPTOR_RSS_ITEM];
    item->node_typei = RAPTOR_RSS_ITEM;
  }

  return 0;
}