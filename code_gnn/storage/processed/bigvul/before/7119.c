  T42_Driver_Init( FT_Module  module )         
  {
    T42_Driver  driver = (T42_Driver)module;
    FT_Module   ttmodule;


    ttmodule = FT_Get_Module( module->library, "truetype" );
    if ( !ttmodule )
    {
      FT_ERROR(( "T42_Driver_Init: cannot access `truetype' module\n" ));
      return FT_THROW( Missing_Module );
    }

    driver->ttclazz = (FT_Driver_Class)ttmodule->clazz;

    return FT_Err_Ok;
  }
