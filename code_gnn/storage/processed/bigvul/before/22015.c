raptor_rdfxml_parse_init(raptor_parser* rdf_parser, const char *name)
{
  raptor_rdfxml_parser* rdf_xml_parser = (raptor_rdfxml_parser*)rdf_parser->context;
  raptor_sax2* sax2;
  raptor_world* world = rdf_parser->world;

   
  sax2 = raptor_new_sax2(rdf_parser->world, &rdf_parser->locator, rdf_parser);
  rdf_xml_parser->sax2 = sax2;
  if(!sax2)
    return 1;

   
  raptor_sax2_set_start_element_handler(sax2, raptor_rdfxml_start_element_handler);
  raptor_sax2_set_end_element_handler(sax2, raptor_rdfxml_end_element_handler);
  raptor_sax2_set_characters_handler(sax2, raptor_rdfxml_characters_handler);
  raptor_sax2_set_cdata_handler(sax2, raptor_rdfxml_cdata_handler);
  raptor_sax2_set_comment_handler(sax2, raptor_rdfxml_comment_handler);
  raptor_sax2_set_namespace_handler(sax2, raptor_rdfxml_sax2_new_namespace_handler);

     
  RAPTOR_DAML_NS_URI(rdf_xml_parser) = raptor_new_uri_from_counted_string(world,
                                                                          daml_namespace_uri_string,
                                                                          daml_namespace_uri_string_len);

  RAPTOR_DAML_List_URI(rdf_xml_parser) = raptor_new_uri_from_uri_local_name(world, RAPTOR_DAML_NS_URI(rdf_xml_parser), (const unsigned char *)"List");
  RAPTOR_DAML_first_URI(rdf_xml_parser) = raptor_new_uri_from_uri_local_name(world, RAPTOR_DAML_NS_URI(rdf_xml_parser) ,(const unsigned char *)"first");
  RAPTOR_DAML_rest_URI(rdf_xml_parser) = raptor_new_uri_from_uri_local_name(world, RAPTOR_DAML_NS_URI(rdf_xml_parser), (const unsigned char *)"rest");
  RAPTOR_DAML_nil_URI(rdf_xml_parser) = raptor_new_uri_from_uri_local_name(world, RAPTOR_DAML_NS_URI(rdf_xml_parser), (const unsigned char *)"nil");

   
  if(!RAPTOR_DAML_NS_URI(rdf_xml_parser) ||
     !RAPTOR_DAML_List_URI(rdf_xml_parser) ||
     !RAPTOR_DAML_first_URI(rdf_xml_parser) ||
     !RAPTOR_DAML_rest_URI(rdf_xml_parser) ||
     !RAPTOR_DAML_nil_URI(rdf_xml_parser))
    return 1;

   
  return 0;
}