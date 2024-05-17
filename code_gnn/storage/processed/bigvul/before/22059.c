raptor_turtle_writer_comment(raptor_turtle_writer* turtle_writer,
                             const unsigned char *string)
{
  unsigned char c;
  size_t len = strlen((const char*)string);

  raptor_iostream_counted_string_write((const unsigned char*)"# ", 2,
                                       turtle_writer->iostr);

  for(; (c=*string); string++, len--) {
    if(c == '\n') {
      raptor_turtle_writer_newline(turtle_writer);
      raptor_iostream_counted_string_write((const unsigned char*)"# ", 2,
                                           turtle_writer->iostr);
    } else if(c != '\r') { 
       
      raptor_iostream_write_byte(c, turtle_writer->iostr);
    }
  }
  
  raptor_turtle_writer_newline(turtle_writer);
}