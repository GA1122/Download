raptor_new_turtle_writer(raptor_world* world,
                         raptor_uri* base_uri, int write_base_uri,
                         raptor_namespace_stack *nstack,
                         raptor_iostream* iostr)
{
  raptor_turtle_writer* turtle_writer;

  RAPTOR_CHECK_CONSTRUCTOR_WORLD(world);

  if(!nstack || !iostr)
    return NULL;
  
  raptor_world_open(world);

  turtle_writer = RAPTOR_CALLOC(raptor_turtle_writer*, 1,
                                sizeof(*turtle_writer));

  if(!turtle_writer)
    return NULL;

  turtle_writer->world = world;

  turtle_writer->nstack_depth = 0;

  turtle_writer->nstack = nstack;
  if(!turtle_writer->nstack) {
    turtle_writer->nstack = raptor_new_namespaces(world, 1);
    turtle_writer->my_nstack = 1;
  }

  turtle_writer->iostr = iostr;

  turtle_writer->flags = 0;
  turtle_writer->indent = 2;

  turtle_writer->base_uri = NULL;
   
  if(base_uri && write_base_uri)
    raptor_turtle_writer_base(turtle_writer, base_uri);
  turtle_writer->base_uri = base_uri;

  turtle_writer->xsd_boolean_uri = raptor_new_uri(world, (const unsigned char*)"http://www.w3.org/2001/XMLSchema#boolean");
  turtle_writer->xsd_decimal_uri = raptor_new_uri(world, (const unsigned char*)"http://www.w3.org/2001/XMLSchema#decimal");
  turtle_writer->xsd_double_uri = raptor_new_uri(world, (const unsigned char*)"http://www.w3.org/2001/XMLSchema#double");
  turtle_writer->xsd_integer_uri = raptor_new_uri(world, (const unsigned char*)"http://www.w3.org/2001/XMLSchema#integer");
  
  return turtle_writer;
}