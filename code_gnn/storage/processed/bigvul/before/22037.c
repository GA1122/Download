raptor_rss_emit_type_triple(raptor_parser* rdf_parser, 
                            raptor_term *resource,
                            raptor_uri *type_uri) 
{
  raptor_rss_parser* rss_parser = (raptor_rss_parser*)rdf_parser->context;
  raptor_term *predicate_term;
  raptor_term *object_term;

  if(!resource) {
    raptor_parser_error(rdf_parser, "RSS node has no identifier");
    return 1;
  }

  rss_parser->statement.subject = resource;
  
  predicate_term = raptor_new_term_from_uri(rdf_parser->world, 
                                            RAPTOR_RDF_type_URI(rdf_parser->world));
  rss_parser->statement.predicate = predicate_term;

  object_term = raptor_new_term_from_uri(rdf_parser->world, type_uri);
  rss_parser->statement.object = object_term;
  
   
  (*rdf_parser->statement_handler)(rdf_parser->user_data, &rss_parser->statement);

  raptor_free_term(predicate_term);
  raptor_free_term(object_term);
  
  return 0;
}