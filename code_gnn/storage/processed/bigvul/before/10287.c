  load_face_in_embedded_rfork( FT_Library           library,
                               FT_Stream            stream,
                               FT_Long              face_index,
                               FT_Face             *aface,
                               const FT_Open_Args  *args )
  {

#undef  FT_COMPONENT
#define FT_COMPONENT  trace_raccess

    FT_Memory  memory = library->memory;
    FT_Error   error  = FT_Err_Unknown_File_Format;
    int        i;

    char *     file_names[FT_RACCESS_N_RULES];
    FT_Long    offsets[FT_RACCESS_N_RULES];
    FT_Error   errors[FT_RACCESS_N_RULES];

    FT_Open_Args  args2;
    FT_Stream     stream2 = 0;


    FT_Raccess_Guess( library, stream,
                      args->pathname, file_names, offsets, errors );

    for ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      if ( errors[i] )
      {
        FT_TRACE3(( "Error[%d] has occurred in rule %d\n", errors[i], i ));
        continue;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_names[i] ? file_names[i] : args->pathname;

      FT_TRACE3(( "Try rule %d: %s (offset=%d) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_Stream_New( library, &args2, &stream2 );
      if ( error )
      {
        FT_TRACE3(( "failed\n" ));
        continue;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_index, aface );
      FT_Stream_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      if ( !error )
          break;
    }

    for (i = 0; i < FT_RACCESS_N_RULES; i++)
    {
      if ( file_names[i] )
        FT_FREE( file_names[i] );
    }

     
    if ( error )
      error = FT_Err_Unknown_File_Format;

    return error;

#undef  FT_COMPONENT
#define FT_COMPONENT  trace_objs

  }
