raptor_rss_emit_connection(raptor_parser* rdf_parser,
                           raptor_term *subject_identifier,
                           raptor_uri* predicate_uri, int predicate_ordinal,
                           raptor_term *object_identifier) 
{
  raptor_rss_parser* rss_parser = (raptor_rss_parser*)rdf_parser->context;
  raptor_uri *puri = NULL;
  raptor_term *predicate_term = NULL;
  
  if(!subject_identifier) {
    raptor_parser_error(rdf_parser, "Connection subject has no identifier");
    return 1;
  }

  rss_parser->statement.subject = subject_identifier;

  if(!predicate_uri) {
     
    puri = raptor_new_uri_from_rdf_ordinal(rdf_parser->world, predicate_ordinal);
    predicate_uri = puri;
  }
  predicate_term = raptor_new_term_from_uri(rdf_parser->world,
                                            predicate_uri);
  rss_parser->statement.predicate = predicate_term;
  rss_parser->statement.object = object_identifier;
  
   
  (*rdf_parser->statement_handler)(rdf_parser->user_data,
                                   &rss_parser->statement);

  raptor_free_term(predicate_term);
  
  if(puri)
    raptor_free_uri(puri);
    
  return 0;
}