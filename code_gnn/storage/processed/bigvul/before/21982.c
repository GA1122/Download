raptor_libxml_xmlStructuredError_handler_global(void *user_data,
                                                xmlErrorPtr err)
{
  raptor_world *world = NULL;
  
   
  if(user_data) {
    world = (raptor_world*)user_data;
    if(world->magic != RAPTOR2_WORLD_MAGIC)
      world = NULL;
  }

  raptor_libxml_xmlStructuredError_handler_common(world, NULL, err);
}