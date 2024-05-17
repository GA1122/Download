raptor_rdfxml_element_has_property_attributes(raptor_rdfxml_element *element) 
{
  int i;
  
  if(element->xml_element->attribute_count > 0)
    return 1;

   
  for(i = 0; i <= RDF_NS_LAST; i++) {
    if(element->rdf_attr[i] &&
       raptor_rdf_ns_terms_info[i].type != RAPTOR_TERM_TYPE_UNKNOWN)
      return 1;
  }
  return 0;
}