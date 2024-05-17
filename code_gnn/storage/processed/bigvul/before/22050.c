raptor_rss_start_namespaces(raptor_parser* rdf_parser)
{
  raptor_rss_parser* rss_parser = (raptor_rss_parser*)rdf_parser->context;
  int i;
  int n;

   
  for(i = 0; i< RAPTOR_RSS_COMMON_SIZE; i++) {
    raptor_rss_item* item;
    
     
    for(item = rss_parser->model.common[i]; item; item = item->next) {
      int f;
      if(!item->fields_count)
        continue;
      
       
      for(f = 0; f< RAPTOR_RSS_FIELDS_SIZE; f++) {
        raptor_rss_field* field;
         
        for(field = item->fields[f]; field; field = field->next) {
          rss_info_namespace ns_index = raptor_rss_fields_info[f].nspace;
          rss_parser->nspaces_seen[ns_index] = 'Y';
           
          break;
        }
      }
    }
  }

   
  for(n = 0; n < RAPTOR_RSS_NAMESPACES_SIZE; n++) {
    if(rss_parser->nspaces[n] && rss_parser->nspaces_seen[n] == 'Y')
      raptor_parser_start_namespace(rdf_parser, rss_parser->nspaces[n]);
  }
}
