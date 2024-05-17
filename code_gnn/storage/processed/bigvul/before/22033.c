raptor_rss_emit(raptor_parser* rdf_parser)
{
  raptor_rss_parser* rss_parser = (raptor_rss_parser*)rdf_parser->context;
  int i;
  raptor_rss_item* item;
  int rc = 0;

  if(!rss_parser->model.common[RAPTOR_RSS_CHANNEL]) {
    raptor_parser_error(rdf_parser, "No RSS channel item present");
    return 1;
  }
  
  if(!rss_parser->model.common[RAPTOR_RSS_CHANNEL]->term) {
    raptor_parser_error(rdf_parser, "RSS channel has no identifier");
    return 1;
  }

   
  raptor_parser_start_graph(rdf_parser, NULL, 0);
  rdf_parser->emitted_default_graph++;


   
  for(i = 0; i< RAPTOR_RSS_COMMON_SIZE; i++) {
    for(item = rss_parser->model.common[i]; item; item = item->next) {
      if(!item->fields_count)
        continue;
      
      RAPTOR_DEBUG3("Emitting type %i - %s\n", i, raptor_rss_items_info[i].name);
      
      if(!item->term) {
        raptor_parser_error(rdf_parser, "RSS %s has no identifier",
                            raptor_rss_items_info[i].name);
        rc = 1;
        goto tidy;
      }
    
      if(raptor_rss_emit_item(rdf_parser, item)) {
        rc = 1;
        goto tidy;
      }

       
      if(i != RAPTOR_RSS_CHANNEL) {
        if(raptor_rss_emit_connection(rdf_parser,
                                      rss_parser->model.common[RAPTOR_RSS_CHANNEL]->term,
                                      rdf_parser->world->rss_types_info_uris[i], 0,
                                      item->term)) {
          rc = 1;
          goto tidy;
        }
      }
    }
  }


   
  if(rss_parser->model.items_count) {
    const unsigned char* id;
    raptor_term *items;

    id = raptor_world_generate_bnodeid(rdf_parser->world);
    
     
    items = raptor_new_term_from_blank(rdf_parser->world, id);
    RAPTOR_FREE(char*, id);

     
    if(raptor_rss_emit_type_triple(rdf_parser, items,
                                   RAPTOR_RDF_Seq_URI(rdf_parser->world))) {
      raptor_free_term(items);
      rc = 1;
      goto tidy;
    }
    
     
    if(raptor_rss_emit_connection(rdf_parser,
                                  rss_parser->model.common[RAPTOR_RSS_CHANNEL]->term,
                                  rdf_parser->world->rss_fields_info_uris[RAPTOR_RSS_FIELD_ITEMS], 0,
                                  items)) {
      raptor_free_term(items);
      rc= 1;
      goto tidy;
    }
    
     
    for(i = 1, item = rss_parser->model.items; item; item = item->next, i++) {
      
      if(raptor_rss_emit_item(rdf_parser, item) ||
         raptor_rss_emit_connection(rdf_parser, items, NULL, i,item->term)) {
        raptor_free_term(items);
        rc = 1;
        goto tidy;
      }
    }

    raptor_free_term(items);
  }

  tidy:
  if(rdf_parser->emitted_default_graph) {
    raptor_parser_end_graph(rdf_parser, NULL, 0);
    rdf_parser->emitted_default_graph--;
  }

  return rc;
}
