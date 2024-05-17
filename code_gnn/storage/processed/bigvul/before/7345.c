  ps_parser_skip_PS_token( PS_Parser  parser )
  {
     
     
     

    FT_Byte*  cur   = parser->cursor;
    FT_Byte*  limit = parser->limit;
    FT_Error  error = FT_Err_Ok;


    skip_spaces( &cur, limit );              
    if ( cur >= limit )
      goto Exit;

     
    if ( *cur == '[' || *cur == ']' )
    {
      cur++;
      goto Exit;
    }

     

    if ( *cur == '{' )                               
    {
      error = skip_procedure( &cur, limit );
      goto Exit;
    }

    if ( *cur == '(' )                               
    {
      error = skip_literal_string( &cur, limit );
      goto Exit;
    }

    if ( *cur == '<' )                               
    {
      if ( cur + 1 < limit && *(cur + 1) == '<' )    
      {
        cur++;
        cur++;
      }
      else
        error = skip_string( &cur, limit );

      goto Exit;
    }

    if ( *cur == '>' )
    {
      cur++;
      if ( cur >= limit || *cur != '>' )              
      {
        FT_ERROR(( "ps_parser_skip_PS_token:"
                   " unexpected closing delimiter `>'\n" ));
        error = FT_THROW( Invalid_File_Format );
        goto Exit;
      }
      cur++;
      goto Exit;
    }

    if ( *cur == '/' )
      cur++;

     
    while ( cur < limit )
    {
       
       
      if ( IS_PS_DELIM( *cur ) )
        break;

      cur++;
    }

  Exit:
    if ( cur < limit && cur == parser->cursor )
    {
      FT_ERROR(( "ps_parser_skip_PS_token:"
                 " current token is `%c' which is self-delimiting\n"
                 "                        "
                 " but invalid at this point\n",
                 *cur ));

      error = FT_THROW( Invalid_File_Format );
    }

    if ( cur > limit )
      cur = limit;

    parser->error  = error;
    parser->cursor = cur;
  }
