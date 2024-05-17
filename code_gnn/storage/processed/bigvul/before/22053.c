main(int argc, char *argv[]) 
{
  raptor_world *world;
  const char *program = raptor_basename(argv[0]);
  raptor_iostream *iostr;
  raptor_namespace_stack *nstack;
  raptor_namespace* ex_ns;
  raptor_turtle_writer* turtle_writer;
  raptor_uri* base_uri;
  raptor_qname* el_name;
  unsigned long count;
  raptor_uri* datatype;
  
   
  void *string = NULL;
  size_t string_len = 0;

  world = raptor_new_world();
  if(!world || raptor_world_open(world))
    exit(1);
  
  iostr = raptor_new_iostream_to_string(world, &string, &string_len, NULL);
  if(!iostr) {
    fprintf(stderr, "%s: Failed to create iostream to string\n", program);
    exit(1);
  }

  nstack = raptor_new_namespaces(world, 1);

  base_uri = raptor_new_uri(world, base_uri_string);

  turtle_writer = raptor_new_turtle_writer(world, base_uri, 1, nstack, iostr);
  if(!turtle_writer) {
    fprintf(stderr, "%s: Failed to create turtle_writer to iostream\n", program);
    exit(1);
  }

  raptor_turtle_writer_set_option(turtle_writer, 
                                   RAPTOR_OPTION_WRITER_AUTO_INDENT, 1);

  ex_ns = raptor_new_namespace(nstack,
                              (const unsigned char*)"ex",
                              (const unsigned char*)"http://example.org/ns#",
                              0);


  raptor_turtle_writer_namespace_prefix(turtle_writer, ex_ns);

  raptor_turtle_writer_reference(turtle_writer, base_uri);
  
  raptor_turtle_writer_increase_indent(turtle_writer);
  raptor_turtle_writer_newline(turtle_writer);
  
  raptor_turtle_writer_raw(turtle_writer, (const unsigned char*)"ex:foo ");

  raptor_turtle_writer_quoted_counted_string(turtle_writer, longstr,
                                             strlen((const char*)longstr));
  raptor_turtle_writer_raw_counted(turtle_writer,
                                   (const unsigned char*)" ;", 2);
  raptor_turtle_writer_newline(turtle_writer);

  el_name = raptor_new_qname_from_namespace_local_name(world,
                                                       ex_ns,
                                                       (const unsigned char*)"bar", 
                                                       NULL);

  raptor_turtle_writer_qname(turtle_writer, el_name);
  raptor_free_qname(el_name);

  raptor_turtle_writer_raw_counted(turtle_writer, (const unsigned char*)" ", 1);

  datatype = raptor_new_uri(world, (const unsigned char*)"http://www.w3.org/2001/XMLSchema#decimal");
  raptor_turtle_writer_literal(turtle_writer, nstack,
                               (const unsigned char*)"10.0", NULL,
                               datatype);
  raptor_free_uri(datatype);

  raptor_turtle_writer_newline(turtle_writer);

  raptor_turtle_writer_decrease_indent(turtle_writer);

  raptor_turtle_writer_raw_counted(turtle_writer, (const unsigned char*)".", 1);
  raptor_turtle_writer_newline(turtle_writer);

  
  raptor_free_turtle_writer(turtle_writer);

  raptor_free_namespace(ex_ns);

  raptor_free_namespaces(nstack);

  raptor_free_uri(base_uri);

  
  count = raptor_iostream_tell(iostr);

#if defined(RAPTOR_DEBUG) && RAPTOR_DEBUG > 1
  fprintf(stderr, "%s: Freeing iostream\n", program);
#endif
  raptor_free_iostream(iostr);

  if(count != OUT_BYTES_COUNT) {
    fprintf(stderr, "%s: I/O stream wrote %d bytes, expected %d\n", program,
            (int)count, (int)OUT_BYTES_COUNT);
    fputs("[[", stderr);
    (void)fwrite(string, 1, string_len, stderr);
    fputs("]]\n", stderr);
    return 1;
  }
  
  if(!string) {
    fprintf(stderr, "%s: I/O stream failed to create a string\n", program);
    return 1;
  }
  string_len = strlen((const char*)string);
  if(string_len != count) {
    fprintf(stderr, "%s: I/O stream created a string length %d, expected %d\n", program, (int)string_len, (int)count);
    return 1;
  }

#if defined(RAPTOR_DEBUG) && RAPTOR_DEBUG > 1
  fprintf(stderr, "%s: Made Turtle string of %d bytes\n", program, (int)string_len);
  fputs("[[", stderr);
  (void)fwrite(string, 1, string_len, stderr);
  fputs("]]\n", stderr);
#endif

  raptor_free_memory(string);

  raptor_free_world(world);

   
  return(0);
}