  t42_parser_init( T42_Parser     parser,
                   FT_Stream      stream,
                   FT_Memory      memory,
                   PSAux_Service  psaux )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Long   size;


    psaux->ps_parser_funcs->init( &parser->root, 0, 0, memory );

    parser->stream    = stream;
    parser->base_len  = 0;
    parser->base_dict = 0;
    parser->in_memory = 0;

     
     
     
     
     
     
     
     
     
     
     
     
     

    if ( FT_STREAM_SEEK( 0L ) ||
         FT_FRAME_ENTER( 17 ) )
      goto Exit;

    if ( ft_memcmp( stream->cursor, "%!PS-TrueTypeFont", 17 ) != 0 )
    {
      FT_TRACE2(( "  not a Type42 font\n" ));
      error = FT_THROW( Unknown_File_Format );
    }

    FT_FRAME_EXIT();

    if ( error || FT_STREAM_SEEK( 0 ) )
      goto Exit;

    size = stream->size;

     
     

     
    if ( !stream->read )
    {
      parser->base_dict = (FT_Byte*)stream->base + stream->pos;
      parser->base_len  = size;
      parser->in_memory = 1;

       
      if ( FT_STREAM_SKIP( size ) )
        goto Exit;
    }
    else
    {
       
      if ( FT_ALLOC( parser->base_dict, size )       ||
           FT_STREAM_READ( parser->base_dict, size ) )
        goto Exit;

      parser->base_len = size;
    }

    parser->root.base   = parser->base_dict;
    parser->root.cursor = parser->base_dict;
    parser->root.limit  = parser->root.cursor + parser->base_len;

  Exit:
    if ( error && !parser->in_memory )
      FT_FREE( parser->base_dict );

    return error;
  }
