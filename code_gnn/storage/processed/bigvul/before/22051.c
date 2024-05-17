raptor_rss_uplift_fields(raptor_rss_parser* rss_parser, raptor_rss_item* item) 
{
  int i;
  
   
  for(i = 0; raptor_atom_to_rss[i].from != RAPTOR_RSS_FIELD_UNKNOWN; i++) {
#ifdef RAPTOR_DEBUG
    raptor_rss_fields_type from_field = raptor_atom_to_rss[i].from;
    raptor_rss_fields_type to_field = raptor_atom_to_rss[i].to;
#endif

    if(raptor_rss_copy_field(rss_parser, item, &raptor_atom_to_rss[i]))
      continue;
    RAPTOR_DEBUG3("Copied field %s to rss field %s\n", 
                  raptor_rss_fields_info[from_field].name,
                  raptor_rss_fields_info[to_field].name);
  }
}