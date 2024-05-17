raptor_libxml_generic_error(void* user_data, const char *msg, ...) 
{
  raptor_world* world = (raptor_world*)user_data;
  va_list args;
  const char* prefix = xml_generic_error_prefix;
  int prefix_length = RAPTOR_BAD_CAST(int, strlen(prefix));
  int length;
  char *nmsg;
  int msg_len;
  
  va_start(args, msg);

  msg_len = RAPTOR_BAD_CAST(int, strlen(msg));
  length = prefix_length + msg_len + 1;
  nmsg = RAPTOR_MALLOC(char*, length);
  if(nmsg) {
    memcpy(nmsg, prefix, prefix_length);  
    memcpy(nmsg + prefix_length, msg, msg_len + 1);  
    if(nmsg[length-1] == '\n')
      nmsg[length-1]='\0';
  }

  raptor_log_error_varargs(world, RAPTOR_LOG_LEVEL_ERROR,
                             NULL,
                           nmsg ? nmsg : msg, 
                           args);
  
  if(nmsg)
    RAPTOR_FREE(char*, nmsg);

  va_end(args);
}