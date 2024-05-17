  FT_Get_Module_Interface( FT_Library   library,
                           const char*  mod_name )
  {
    FT_Module  module;


     

    module = FT_Get_Module( library, mod_name );

    return module ? module->clazz->module_interface : 0;
  }
