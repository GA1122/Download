raptor_turtle_writer_literal(raptor_turtle_writer* turtle_writer,
                             raptor_namespace_stack *nstack,
                             const unsigned char* s, const unsigned char* lang,
                             raptor_uri* datatype)
{
   
  #define INT_MAX_LEN 309 

   
  #define FRAC_MAX_LEN 52

  char* endptr = (char *)s;
  int written = 0;

   
  if(datatype) {
     
    if(raptor_uri_equals(datatype, turtle_writer->xsd_integer_uri)) {
       
      long gcc_is_stupid = strtol((const char*)s, &endptr, 10);
      if(endptr != (char*)s && !*endptr) {
        raptor_iostream_string_write(s, turtle_writer->iostr);
         
        written = 1 + 0 * (int)gcc_is_stupid;
      } else {
        raptor_log_error(turtle_writer->world, RAPTOR_LOG_LEVEL_ERROR, NULL,
                         "Illegal value for xsd:integer literal.");
      }

     
    } else if(raptor_uri_equals(datatype, turtle_writer->xsd_double_uri) ||
      raptor_uri_equals(datatype, turtle_writer->xsd_decimal_uri)) {
       
      double gcc_is_doubly_stupid = strtod((const char*)s, &endptr);
      if(endptr != (char*)s && !*endptr) {
        raptor_iostream_string_write(s, turtle_writer->iostr);
         
        written = 1 +  0 * (int)gcc_is_doubly_stupid;
      } else {
        raptor_log_error(turtle_writer->world, RAPTOR_LOG_LEVEL_ERROR, NULL,
                         "Illegal value for xsd:double or xsd:decimal literal.");
      }

     
    } else if(raptor_uri_equals(datatype, turtle_writer->xsd_boolean_uri)) {
      if(!strcmp((const char*)s, "0") || !strcmp((const char*)s, "false")) {
        raptor_iostream_string_write("false", turtle_writer->iostr);
        written = 1;
      } else if(!strcmp((const char*)s, "1") || !strcmp((const char*)s, "true")) {
        raptor_iostream_string_write("true", turtle_writer->iostr);
        written = 1;
      } else {
        raptor_log_error(turtle_writer->world, RAPTOR_LOG_LEVEL_ERROR, NULL,
                         "Illegal value for xsd:boolean literal.");
      }
    }
  }

  if(written)
    return 0;
    
  if(raptor_turtle_writer_quoted_counted_string(turtle_writer, s,
                                                strlen((const char*)s)))
    return 1;

   
  if(datatype) {
    raptor_qname* qname;

    raptor_iostream_string_write("^^", turtle_writer->iostr);
    qname = raptor_new_qname_from_namespace_uri(nstack, datatype, 10);
    if(qname) {
      raptor_turtle_writer_qname(turtle_writer, qname);
      raptor_free_qname(qname);
    } else
      raptor_turtle_writer_reference(turtle_writer, datatype);
  } else if(lang) {
     
    raptor_iostream_write_byte('@', turtle_writer->iostr);
    raptor_iostream_string_write(lang, turtle_writer->iostr);
  }

  return 0;
}