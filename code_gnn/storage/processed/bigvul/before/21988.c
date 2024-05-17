raptor_object_options_init(raptor_object_options* options,
                           raptor_option_area area)
{
  int i;
  
  options->area = area;

  for(i = 0; i <= RAPTOR_OPTION_LAST; i++) {
    if(raptor_option_value_is_numeric((raptor_option)i))
      options->options[i].integer = 0;
    else
      options->options[i].string = NULL;
  }

   

   
  options->options[RAPTOR_OPTION_WRITE_BASE_URI].integer = 1;
  
   
  options->options[RAPTOR_OPTION_RELATIVE_URIS].integer = 1;

   
  options->options[RAPTOR_OPTION_WRITER_XML_VERSION].integer = 10;

   
  options->options[RAPTOR_OPTION_WRITER_XML_DECLARATION].integer = 1;

   
  options->options[RAPTOR_OPTION_WRITER_INDENT_WIDTH].integer = 2;

   
  options->options[RAPTOR_OPTION_STRICT].integer = 0;

   
  options->options[RAPTOR_OPTION_WWW_SSL_VERIFY_PEER].integer = 1;

   
  options->options[RAPTOR_OPTION_WWW_SSL_VERIFY_HOST].integer = 2;

}
