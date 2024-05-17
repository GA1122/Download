  TT_New_Context( TT_Driver  driver )
  {
    FT_Memory  memory;
    FT_Error   error;

    TT_ExecContext  exec = NULL;


    if ( !driver )
      goto Fail;

    memory = driver->root.root.memory;

     
    if ( FT_NEW( exec ) )
      goto Fail;

     
    error = Init_Context( exec, memory );
    if ( error )
      goto Fail;

    return exec;

  Fail:
    return NULL;
  }
