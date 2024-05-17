raptor_string_python_write(const unsigned char *string,
                           size_t len,
                           const char delim,
                           int flags,
                           raptor_iostream *iostr)
{
  unsigned char c;
  int unichar_len;
  raptor_unichar unichar;

  if(flags < 0 || flags > 3)
    return 1;
  
  for(; (c=*string); string++, len--) {
    if((delim && c == delim && (delim == '\'' || delim == '"')) ||
       c == '\\') {
      raptor_iostream_write_byte('\\', iostr);
      raptor_iostream_write_byte(c, iostr);
      continue;
    }
    if(delim && c == delim) {
      raptor_iostream_counted_string_write("\\u", 2, iostr);
      raptor_iostream_hexadecimal_write(c, 4, iostr);
      continue;
    }
    
    if(flags != 2) {
       

       
      if(c == 0x09) {
        raptor_iostream_counted_string_write("\\t", 2, iostr);
        continue;
      } else if((flags == 3) && c == 0x08) {
         
        raptor_iostream_counted_string_write("\\b", 2, iostr);
        continue;
      } else if(c == 0x0a) {
        raptor_iostream_counted_string_write("\\n", 2, iostr);
        continue;
      } else if((flags == 3) && c == 0x0b) {
         
        raptor_iostream_counted_string_write("\\f", 2, iostr);
        continue;
      } else if(c == 0x0d) {
        raptor_iostream_counted_string_write("\\r", 2, iostr);
        continue;
      } else if(c < 0x20|| c == 0x7f) {
        raptor_iostream_counted_string_write("\\u", 2, iostr);
        raptor_iostream_hexadecimal_write(c, 4, iostr);
        continue;
      } else if(c < 0x80) {
        raptor_iostream_write_byte(c, iostr);
        continue;
      }
    } else if(c < 0x80) {
       
      raptor_iostream_write_byte(c, iostr);
      continue;
    } 
    
     
    
    unichar_len = raptor_unicode_utf8_string_get_char(string, len, NULL);
    if(unichar_len < 0 || RAPTOR_GOOD_CAST(size_t, unichar_len) > len)
       
      return 1;

    if(flags >= 1 && flags <= 3) {
       
      raptor_iostream_counted_string_write(string, unichar_len, iostr);
    } else {
      unichar_len = raptor_unicode_utf8_string_get_char(string, len, &unichar);
      if(unichar_len < 0)
        return 1;

      if(unichar < 0x10000) {
        raptor_iostream_counted_string_write("\\u", 2, iostr);
        raptor_iostream_hexadecimal_write(RAPTOR_GOOD_CAST(unsigned int, unichar), 4, iostr);
      } else {
        raptor_iostream_counted_string_write("\\U", 2, iostr);
        raptor_iostream_hexadecimal_write(RAPTOR_GOOD_CAST(unsigned int, unichar), 8, iostr);
      }
    }
    
    unichar_len--;  
    string += unichar_len; len -= unichar_len;

  }

  return 0;
}