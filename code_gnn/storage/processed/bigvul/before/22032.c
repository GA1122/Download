raptor_rss_copy_field(raptor_rss_parser* rss_parser,
                      raptor_rss_item* item,
                      const raptor_field_pair* pair)
{
  raptor_rss_fields_type from_field = pair->from;
  raptor_rss_fields_type to_field = pair->to;
  raptor_rss_field* field = NULL;

  if(!(item->fields[from_field] && item->fields[from_field]->value))
    return 1;

  if(from_field == to_field) {
    field = item->fields[from_field];
  } else {
    if(item->fields[to_field] && item->fields[to_field]->value)
      return 1;
  
    field = raptor_rss_new_field(item->world);
    field->is_mapped = 1;
    raptor_rss_item_add_field(item, to_field, field);
  }
  
   
  rss_parser->nspaces_seen[raptor_rss_fields_info[to_field].nspace] = 'Y';
    
  if(!field->value) {
    if(pair->conversion)
      pair->conversion(item->fields[from_field], field);
    else {
      size_t len;

       
      len = strlen((const char*)item->fields[from_field]->value);

      field->value = RAPTOR_MALLOC(unsigned char*, len + 1);
      if(!field->value)
        return 1;
      
      memcpy(field->value, item->fields[from_field]->value, len + 1);
    }
  }

  return 0;
}