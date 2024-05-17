  cff_index_get_sid_string( CFF_Index           idx,
                            FT_UInt             sid,
                            FT_Service_PsCMaps  psnames )
  {
     
    if ( sid == 0xFFFFU )
      return 0;

     
    if ( sid > 390 )
      return cff_index_get_name( idx, sid - 391 );

     
    if ( !psnames )
      return 0;

     
    {
      FT_String*   name       = 0;
      const char*  adobe_name = psnames->adobe_std_strings( sid );


      if ( adobe_name )
      {
        FT_Memory  memory = idx->stream->memory;
        FT_Error   error;


        (void)FT_STRDUP( name, adobe_name );

        FT_UNUSED( error );
      }

      return name;
    }
  }
