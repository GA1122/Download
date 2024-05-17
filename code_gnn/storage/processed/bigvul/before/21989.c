raptor_object_options_set_option(raptor_object_options *options,
                                 raptor_option option,
                                 const char* string, int integer)
{
  if(!raptor_option_is_valid_for_area(option, options->area))
    return 1;
  
  if(raptor_option_value_is_numeric(option)) {
     
    if(string)
      integer = atoi((const char*)string);

    options->options[(int)option].integer = integer;
    return 0;
  } else {
     
    char *string_copy;
    size_t len = 0;
    
    if(string)
      len = strlen((const char*)string);
    string_copy = RAPTOR_MALLOC(char*, len + 1);
    if(!string_copy)
      return 1;
  
    if(len)
      memcpy(string_copy, string, len);
    string_copy[len] = '\0';
    
    options->options[(int)option].string = string_copy;
  }
  
  return 0;
}