raptor_rss_insert_rss_link(raptor_parser* rdf_parser,
                          raptor_rss_item* item) 
{
  raptor_rss_block *block;
  raptor_rss_field* id_field;
  raptor_rss_field* field = NULL;

   
  id_field = item->fields[RAPTOR_RSS_FIELD_ATOM_ID];
  if(id_field && id_field->value) {
    const char *value = (const char*)id_field->value;
    size_t len = strlen(value);

    field = raptor_rss_new_field(item->world);
    if(!field)
      return 1;
    
    field->value = RAPTOR_MALLOC(unsigned char*, len + 1);
    if(!field->value)
      return 1;
    
    memcpy(field->value, value, len + 1);
    raptor_rss_item_add_field(item, RAPTOR_RSS_FIELD_LINK, field);

    return 0;
  }
  
  
  for(block = item->blocks; block; block = block->next) {
    if(block->rss_type != RAPTOR_ATOM_LINK)
      continue;
    
     
    if(!block->urls[RAPTOR_RSS_LINK_HREF_URL_OFFSET] || 
       (block->strings[RAPTOR_RSS_LINK_REL_STRING_OFFSET] &&
        strcmp(block->strings[RAPTOR_RSS_LINK_REL_STRING_OFFSET], "self"))
       )
      continue;
    
     
    field = raptor_rss_new_field(item->world);
    field->value = raptor_uri_to_string(block->urls[0]);

    raptor_rss_item_add_field(item, RAPTOR_RSS_FIELD_LINK, field);
    return 0;
  }

  return 0;
}