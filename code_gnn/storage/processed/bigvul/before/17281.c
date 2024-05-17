  T1_New_Parser( T1_Parser      parser,
                 FT_Stream      stream,
                 FT_Memory      memory,
                 PSAux_Service  psaux )
  {
    FT_Error   error;
    FT_UShort  tag;
    FT_ULong   size;


    psaux->ps_parser_funcs->init( &parser->root, NULL, NULL, memory );

    parser->stream       = stream;
    parser->base_len     = 0;
    parser->base_dict    = NULL;
    parser->private_len  = 0;
    parser->private_dict = NULL;
    parser->in_pfb       = 0;
    parser->in_memory    = 0;
    parser->single_block = 0;

     
    error = check_type1_format( stream, "%!PS-AdobeFont", 14 );
    if ( error )
    {
      if ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        goto Exit;

      error = check_type1_format( stream, "%!FontType", 10 );
      if ( error )
      {
        FT_TRACE2(( "  not a Type 1 font\n" ));
        goto Exit;
      }
    }

     
     
     
     
     
     
     
     
     
     
     
     
     
     
     

     
     
    if ( FT_STREAM_SEEK( 0L ) )
      goto Exit;

    error = read_pfb_tag( stream, &tag, &size );
    if ( error )
      goto Exit;

    if ( tag != 0x8001U )
    {
       
       
      if ( FT_STREAM_SEEK( 0L ) )
        goto Exit;
      size = stream->size;
    }
    else
      parser->in_pfb = 1;

     
     

     
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
