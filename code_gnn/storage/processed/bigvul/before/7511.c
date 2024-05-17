  tt_driver_init( FT_Module  ttdriver )      
  {

#ifdef TT_USE_BYTECODE_INTERPRETER

    TT_Driver  driver = (TT_Driver)ttdriver;

    driver->interpreter_version = TT_INTERPRETER_VERSION_35;
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    driver->interpreter_version = TT_INTERPRETER_VERSION_38;
#endif
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
    driver->interpreter_version = TT_INTERPRETER_VERSION_40;
#endif

#else  

    FT_UNUSED( ttdriver );

#endif  

    return FT_Err_Ok;
  }
