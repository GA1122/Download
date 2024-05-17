raptor_turtle_writer_quoted_counted_string(raptor_turtle_writer* turtle_writer,
                                           const unsigned char *s, size_t len)
{
  const unsigned char *quotes = (const unsigned char *)"\"\"\"\"";
  const unsigned char *q;
  size_t q_len;
  int flags;
  int rc = 0;

  if(!s)
    return 1;
  
   
  flags = raptor_turtle_writer_contains_newline(s) ? 2 : 1;
  q = (flags == 2) ? quotes : quotes + 2;
  q_len = (q == quotes) ? 3 : 1;
  raptor_iostream_counted_string_write(q, q_len, turtle_writer->iostr);
  rc = raptor_string_python_write(s, strlen((const char*)s), '"', flags,
                                  turtle_writer->iostr);
  raptor_iostream_counted_string_write(q, q_len, turtle_writer->iostr);

  return rc;
}