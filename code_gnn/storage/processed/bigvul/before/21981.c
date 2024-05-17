raptor_libxml_xmlStructuredError_handler_common(raptor_world *world,
                                                raptor_locator *locator,
                                                xmlErrorPtr err)
{
  raptor_stringbuffer* sb;
  char *nmsg;
  raptor_log_level level = RAPTOR_LOG_LEVEL_ERROR;

  if(err == NULL || err->code == XML_ERR_OK || err->level == XML_ERR_NONE)
    return;

   
  if(err->level == XML_ERR_WARNING && !err->file)
    return;

   
  if(err->level == XML_ERR_FATAL)
    err->level = XML_ERR_ERROR;
  

  sb = raptor_new_stringbuffer();
  if(err->domain != XML_FROM_HTML)
    raptor_stringbuffer_append_counted_string(sb, (const unsigned char*)"XML ",
                                              4, 1);
  
  if(err->domain != XML_FROM_NONE && err->domain < XML_LAST_DL) {
    const unsigned char* label;
    label = (const unsigned char*)raptor_libxml_domain_labels[(int)err->domain];
    raptor_stringbuffer_append_string(sb, label, 1);
    raptor_stringbuffer_append_counted_string(sb, 
                                              (const unsigned char*)" ", 1, 1);
  }
  
  if(err->level == XML_ERR_WARNING)
    raptor_stringbuffer_append_counted_string(sb, 
                                              (const unsigned char*)"warning: ", 
                                              9, 1);
  else  
    raptor_stringbuffer_append_counted_string(sb, (const unsigned char*)"error: ", 
                                              7, 1);
  
  if(err->message) {
    unsigned char* msg;
    size_t len;
    msg = (unsigned char*)err->message;
    len= strlen((const char*)msg);
    if(len && msg[len-1] == '\n')
      msg[--len]='\0';
    
    raptor_stringbuffer_append_counted_string(sb, msg, len, 1);
  }

#if LIBXML_VERSION >= 20618
   

   
  if(err->domain == XML_FROM_HTTP && err->str1) {
    unsigned char* msg;
    size_t len;
    msg = (unsigned char*)err->str1;
    len= strlen((const char*)msg);
    if(len && msg[len-1] == '\n')
      msg[--len]='\0';
    
    raptor_stringbuffer_append_counted_string(sb, (const unsigned char*)" - ",
                                              3, 1);
    raptor_stringbuffer_append_counted_string(sb, msg, len, 1);
  }
#endif
  
   
  if(err->domain == XML_FROM_XPATH && err->str1) {
    raptor_stringbuffer_append_counted_string(sb, (const unsigned char*)" in ",
                                              4, 1);
    raptor_stringbuffer_append_string(sb, (const unsigned char*)err->str1, 1);
  }

  nmsg = (char*)raptor_stringbuffer_as_string(sb);
  if(err->level == XML_ERR_FATAL)
    level = RAPTOR_LOG_LEVEL_FATAL;
  else if(err->level == XML_ERR_ERROR)
    level = RAPTOR_LOG_LEVEL_ERROR;
  else
    level = RAPTOR_LOG_LEVEL_WARN;

  raptor_log_error(world, level, locator, nmsg);
  
  raptor_free_stringbuffer(sb);
}