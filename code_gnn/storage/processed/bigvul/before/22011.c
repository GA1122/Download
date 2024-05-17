raptor_rdfxml_generate_statement(raptor_parser *rdf_parser, 
                                 raptor_term *subject_term,
                                 raptor_uri *predicate_uri,
                                 raptor_term *object_term,
                                 raptor_term *reified_term,
                                 raptor_rdfxml_element* bag_element)
{
  raptor_statement *statement = &rdf_parser->statement;
  raptor_term* predicate_term = NULL;
  int free_reified_term = 0;
  
  if(rdf_parser->failed)
    return;

#ifdef RAPTOR_DEBUG_VERBOSE
  if(!subject_term)
    RAPTOR_FATAL1("Statement has no subject\n");
  
  if(!predicate_uri)
    RAPTOR_FATAL1("Statement has no predicate\n");
  
  if(!object_term)
    RAPTOR_FATAL1("Statement has no object\n");
  
#endif

  predicate_term = raptor_new_term_from_uri(rdf_parser->world, predicate_uri);
  if(!predicate_term)
    return;

  statement->subject = subject_term;
  statement->predicate = predicate_term;
  statement->object = object_term;

#ifdef RAPTOR_DEBUG_VERBOSE
  fprintf(stderr, "raptor_rdfxml_generate_statement: Generating statement: ");
  raptor_statement_print(statement, stderr);
  fputc('\n', stderr);
#endif

  if(!rdf_parser->emitted_default_graph) {
    raptor_parser_start_graph(rdf_parser, NULL, 0);
    rdf_parser->emitted_default_graph++;
  }

  if(!rdf_parser->statement_handler)
    goto generate_tidy;

   
  (*rdf_parser->statement_handler)(rdf_parser->user_data, statement);


   
  if(RAPTOR_OPTIONS_GET_NUMERIC(rdf_parser, RAPTOR_OPTION_ALLOW_BAGID) &&
     bag_element && bag_element->bag) {
    raptor_term* bag = bag_element->bag;
    raptor_uri* bag_predicate_uri = NULL;
    raptor_term* bag_predicate_term = NULL;
    
    statement->subject = bag;

    bag_element->last_bag_ordinal++;

     
    bag_predicate_uri = raptor_new_uri_from_rdf_ordinal(rdf_parser->world,
                                                        bag_element->last_bag_ordinal);
    if(!bag_predicate_uri)
      goto generate_tidy;

    bag_predicate_term = raptor_new_term_from_uri(rdf_parser->world,
                                                  bag_predicate_uri);
    raptor_free_uri(bag_predicate_uri);

    if(!bag_predicate_term)
      goto generate_tidy;
    
    statement->predicate = bag_predicate_term;

    if(!reified_term || !reified_term->value.blank.string) {
      unsigned char *reified_id = NULL;

       
      reified_id = raptor_world_generate_bnodeid(rdf_parser->world);
      if(!reified_id)
        goto generate_tidy;

      reified_term = raptor_new_term_from_blank(rdf_parser->world, reified_id);
      RAPTOR_FREE(char*, reified_id);

      if(!reified_term)
        goto generate_tidy;
      free_reified_term = 1;
    }
    
    statement->object = reified_term;
    (*rdf_parser->statement_handler)(rdf_parser->user_data, statement);

    if(bag_predicate_term)
      raptor_free_term(bag_predicate_term);
  }


   
  if(!reified_term || !reified_term->value.blank.string)
    goto generate_tidy;


   

  statement->subject = reified_term;
  statement->predicate = RAPTOR_RDF_type_term(rdf_parser->world);
  statement->object = RAPTOR_RDF_Statement_term(rdf_parser->world);
  (*rdf_parser->statement_handler)(rdf_parser->user_data, statement);

   
  statement->predicate = RAPTOR_RDF_subject_term(rdf_parser->world);
  statement->object = subject_term;
  (*rdf_parser->statement_handler)(rdf_parser->user_data, statement);


   
  statement->predicate = RAPTOR_RDF_predicate_term(rdf_parser->world);
  statement->object = predicate_term;
  (*rdf_parser->statement_handler)(rdf_parser->user_data, statement);

   
  statement->predicate = RAPTOR_RDF_object_term(rdf_parser->world);
  statement->object = object_term;
  (*rdf_parser->statement_handler)(rdf_parser->user_data, statement);


 generate_tidy:
   
  if(predicate_term)
    raptor_free_term(predicate_term);
  if(free_reified_term && reified_term)
    raptor_free_term(reified_term);
}