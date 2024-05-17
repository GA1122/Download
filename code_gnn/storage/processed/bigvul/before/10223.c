  Destroy_Module( FT_Module  module )
  {
    FT_Memory         memory  = module->memory;
    FT_Module_Class*  clazz   = module->clazz;
    FT_Library        library = module->library;


     
    if ( module->generic.finalizer )
      module->generic.finalizer( module );

    if ( library && library->auto_hinter == module )
      library->auto_hinter = 0;

     
    if ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

     
    if ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

     
    if ( clazz->module_done )
      clazz->module_done( module );

     
    FT_FREE( module );
  }
