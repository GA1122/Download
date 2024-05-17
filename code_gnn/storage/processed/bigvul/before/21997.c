raptor_free_rdfxml_element(raptor_rdfxml_element *element)
{
  int i;
  
   
  for(i = 0; i <= RDF_NS_LAST; i++) 
    if(element->rdf_attr[i])
      RAPTOR_FREE(char*, element->rdf_attr[i]);

  if(element->subject)
    raptor_free_term(element->subject);
  if(element->predicate)
    raptor_free_term(element->predicate);
  if(element->object)
    raptor_free_term(element->object);
  if(element->bag)
    raptor_free_term(element->bag);
  if(element->reified)
    raptor_free_term(element->reified);

  if(element->tail_id)
    RAPTOR_FREE(char*, (char*)element->tail_id);
  if(element->object_literal_datatype)
    raptor_free_uri(element->object_literal_datatype);

  if(element->reified_id)
    RAPTOR_FREE(char*, (char*)element->reified_id);

  RAPTOR_FREE(raptor_rdfxml_element, element);
}