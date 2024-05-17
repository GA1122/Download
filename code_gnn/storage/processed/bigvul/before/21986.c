raptor_object_options_copy_state(raptor_object_options* to,
                                 raptor_object_options* from)
{
  int rc = 0;
  int i;
  
  to->area = from->area;
  for(i = 0; !rc && i <= RAPTOR_OPTION_LAST; i++) {
    if(raptor_option_value_is_numeric((raptor_option)i))
      to->options[i].integer = from->options[i].integer;
    else {
       
      char* string = from->options[i].string;
      if(string) {
        size_t len = strlen(string);
        to->options[i].string = RAPTOR_MALLOC(char*, len + 1);
        if(to->options[i].string)
          memcpy(to->options[i].string, string, len + 1);
        else
          rc = 1;
      }
    }
  }
  
  return rc;
}