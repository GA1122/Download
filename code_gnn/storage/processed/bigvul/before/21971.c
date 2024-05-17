raptor_libxml_init(raptor_world* world)
{
  xmlInitParser();

  if(world->libxml_flags & RAPTOR_WORLD_FLAG_LIBXML_STRUCTURED_ERROR_SAVE) {
    world->libxml_saved_structured_error_context = xmlGenericErrorContext;
    world->libxml_saved_structured_error_handler = xmlStructuredError;
     
    xmlSetStructuredErrorFunc(world, 
                              (xmlStructuredErrorFunc)raptor_libxml_xmlStructuredError_handler_global);
  }
  
  if(world->libxml_flags & RAPTOR_WORLD_FLAG_LIBXML_GENERIC_ERROR_SAVE) {
    world->libxml_saved_generic_error_context = xmlGenericErrorContext;
    world->libxml_saved_generic_error_handler = xmlGenericError;
     
    xmlSetGenericErrorFunc(world, 
                           (xmlGenericErrorFunc)raptor_libxml_generic_error);
  }

  return 0;
}