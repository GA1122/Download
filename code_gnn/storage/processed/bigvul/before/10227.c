  FT_Attach_Stream( FT_Face        face,
                    FT_Open_Args*  parameters )
  {
    FT_Stream  stream;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


     

    if ( !face )
      return FT_Err_Invalid_Face_Handle;

    driver = face->driver;
    if ( !driver )
      return FT_Err_Invalid_Driver_Handle;

    error = FT_Stream_New( driver->root.library, parameters, &stream );
    if ( error )
      goto Exit;

     
     

    error = FT_Err_Unimplemented_Feature;
    clazz = driver->clazz;
    if ( clazz->attach_file )
      error = clazz->attach_file( face, stream );

     
    FT_Stream_Free( stream,
                    (FT_Bool)( parameters->stream &&
                               ( parameters->flags & FT_OPEN_STREAM ) ) );

  Exit:
    return error;
  }