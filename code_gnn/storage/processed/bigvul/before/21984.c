raptor_free_option_description(raptor_option_description* option_description)
{
  if(!option_description)
    return;

   
   
   

  if(option_description->uri)
    raptor_free_uri(option_description->uri);

  RAPTOR_FREE(raptor_option_description, option_description);
}
