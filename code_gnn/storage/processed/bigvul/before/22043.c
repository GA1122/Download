raptor_rss_parse_init(raptor_parser* rdf_parser, const char *name)
{
  raptor_rss_parser* rss_parser = (raptor_rss_parser*)rdf_parser->context;
  raptor_sax2* sax2;
  int n;

  raptor_rss_common_init(rdf_parser->world);

  raptor_rss_model_init(rdf_parser->world, &rss_parser->model);

  rss_parser->prev_type = RAPTOR_RSS_NONE;
  rss_parser->current_field = RAPTOR_RSS_FIELD_NONE;
  rss_parser->current_type = RAPTOR_RSS_NONE;
  rss_parser->current_block = NULL;

  if(rss_parser->sax2) {
    raptor_free_sax2(rss_parser->sax2);
    rss_parser->sax2 = NULL;
  }

  rss_parser->nstack = raptor_new_namespaces(rdf_parser->world, 1);

   
  for(n = 0; n < RAPTOR_RSS_NAMESPACES_SIZE; n++) {
    unsigned const char* prefix;
    raptor_uri* uri;
    raptor_namespace* nspace = NULL;

    prefix = (unsigned const char*)raptor_rss_namespaces_info[n].prefix;
    uri = rdf_parser->world->rss_namespaces_info_uris[n];
    if(prefix && uri)
      nspace = raptor_new_namespace_from_uri(rss_parser->nstack,
                                             prefix, uri, 0);
    rss_parser->nspaces[n] = nspace;
  }

  sax2 = raptor_new_sax2(rdf_parser->world, &rdf_parser->locator, rdf_parser);
  rss_parser->sax2 = sax2;

  raptor_sax2_set_start_element_handler(sax2, raptor_rss_start_element_handler);
  raptor_sax2_set_end_element_handler(sax2, raptor_rss_end_element_handler);
  raptor_sax2_set_characters_handler(sax2, raptor_rss_cdata_handler);
  raptor_sax2_set_cdata_handler(sax2, raptor_rss_cdata_handler);
  raptor_sax2_set_comment_handler(sax2, raptor_rss_comment_handler);
  raptor_sax2_set_namespace_handler(sax2, raptor_rss_sax2_new_namespace_handler);

  raptor_statement_init(&rss_parser->statement, rdf_parser->world);
  
  return 0;
}