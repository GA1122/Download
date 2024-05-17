raptor_rss_add_container(raptor_rss_parser *rss_parser, const char *name) 
{
  raptor_rss_type type = RAPTOR_RSS_NONE;
    
  if(!strcmp(name, "rss") || !strcmp(name, "rdf") || !strcmp(name, "RDF")) {
     
  } else if(!raptor_strcasecmp(name, "channel")) {
     
    type = RAPTOR_RSS_CHANNEL;
  } else if(!strcmp(name, "feed")) {
     
    type = RAPTOR_RSS_CHANNEL;
    rss_parser->is_atom = 1;
  } else if(!strcmp(name, "item")) {
    type = RAPTOR_RSS_ITEM;
  } else if(!strcmp(name, "entry")) {
    type = RAPTOR_RSS_ITEM;
    rss_parser->is_atom = 1;
  } else {
    int i;
    for(i = 0; i < RAPTOR_RSS_COMMON_SIZE; i++) {
      if(!(raptor_rss_items_info[i].flags & RAPTOR_RSS_ITEM_CONTAINER))
        continue;
    
      if(!strcmp(name, raptor_rss_items_info[i].name)) {
         
        if(i != RAPTOR_ATOM_AUTHOR ||
           (i == RAPTOR_ATOM_AUTHOR && rss_parser->is_atom)) {
          type = (raptor_rss_type)i;
          break;
        }
      }
    }
  }
    
  if(type != RAPTOR_RSS_NONE) {
    if(type == RAPTOR_RSS_ITEM)
      raptor_rss_model_add_item(&rss_parser->model);
    else
      raptor_rss_model_add_common(&rss_parser->model, type);

     
    if(rss_parser->current_type != RAPTOR_RSS_NONE)
      rss_parser->prev_type = rss_parser->current_type;
      
    rss_parser->current_type = type;
  }
  
  return (type == RAPTOR_RSS_NONE);
}
