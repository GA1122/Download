raptor_rss_emit_item(raptor_parser* rdf_parser, raptor_rss_item *item) 
{
  raptor_rss_parser* rss_parser = (raptor_rss_parser*)rdf_parser->context;
  int f;
  raptor_rss_block *block;
  raptor_uri *type_uri;

  if(!item->fields_count)
    return 0;

   
  if(item->node_typei == RAPTOR_ATOM_AUTHOR) 
    type_uri = rdf_parser->world->rss_fields_info_uris[RAPTOR_RSS_RDF_ATOM_AUTHOR_CLASS];
  else
    type_uri = rdf_parser->world->rss_types_info_uris[item->node_typei];

  if(raptor_rss_emit_type_triple(rdf_parser, item->term, type_uri))
    return 1;

  for(f = 0; f< RAPTOR_RSS_FIELDS_SIZE; f++) {
    raptor_rss_field* field;
    raptor_uri* predicate_uri = NULL;
    raptor_term* predicate_term = NULL;
    
     	  
    if(f == RAPTOR_RSS_FIELD_ITEMS)
      continue;

     
    predicate_uri = rdf_parser->world->rss_fields_info_uris[f];
    if(!predicate_uri)
      continue;
    
    predicate_term = raptor_new_term_from_uri(rdf_parser->world,
                                              predicate_uri);
    if(!predicate_term)
      continue;

    rss_parser->statement.predicate = predicate_term;
    
    for(field = item->fields[f]; field; field = field->next) {
      raptor_term* object_term;

      if(field->value) {
         
        object_term = raptor_new_term_from_literal(rdf_parser->world,
                                                   field->value,
                                                   NULL, NULL);
      } else {
        object_term = raptor_new_term_from_uri(rdf_parser->world,
                                               field->uri);
      }
      rss_parser->statement.object = object_term;
      
       
      (*rdf_parser->statement_handler)(rdf_parser->user_data,
                                       &rss_parser->statement);

      raptor_free_term(object_term);
    }

    raptor_free_term(predicate_term);
  }

  for(block = item->blocks; block; block = block->next) {
    raptor_rss_emit_block(rdf_parser, item->term, block);
  }

  return 0;
}