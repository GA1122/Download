  TT_New_Context( TT_Driver  driver )
  {
    TT_ExecContext  exec;
    FT_Memory       memory;


    memory = driver->root.root.memory;
    exec   = driver->context;

    if ( !driver->context )
    {
      FT_Error  error;


       
      if ( FT_NEW( exec ) )
        goto Fail;

       
      error = Init_Context( exec, memory );
      if ( error )
        goto Fail;

       
      driver->context = exec;
    }

    return driver->context;

  Fail:
    return NULL;
  }
