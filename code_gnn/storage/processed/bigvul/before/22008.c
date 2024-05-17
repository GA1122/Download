raptor_rdfxml_element_pop(raptor_rdfxml_parser *rdf_xml_parser) 
{
  raptor_rdfxml_element *element = rdf_xml_parser->current_element;

  if(!element)
    return NULL;

  rdf_xml_parser->current_element = element->parent;
  if(rdf_xml_parser->root_element == element)  
    rdf_xml_parser->root_element = NULL;

  return element;
}