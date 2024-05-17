  t42_parse_dict( T42_Face    face,
                  T42_Loader  loader,
                  FT_Byte*    base,
                  FT_Long     size )
  {
    T42_Parser  parser     = &loader->parser;
    FT_Byte*    limit;
    FT_Int      n_keywords = (FT_Int)( sizeof ( t42_keywords ) /
                                         sizeof ( t42_keywords[0] ) );


    parser->root.cursor = base;
    parser->root.limit  = base + size;
    parser->root.error  = FT_Err_Ok;

    limit = parser->root.limit;

    T1_Skip_Spaces( parser );

    while ( parser->root.cursor < limit )
    {
      FT_Byte*  cur;


      cur = parser->root.cursor;

       
      if ( *cur == 'F' && cur + 25 < limit                    &&
           ft_strncmp( (char*)cur, "FontDirectory", 13 ) == 0 )
      {
        FT_Byte*  cur2;


         
        T1_Skip_PS_Token( parser );
        T1_Skip_Spaces  ( parser );
        cur = cur2 = parser->root.cursor;

         
        while ( cur < limit )
        {
          if ( *cur == 'k' && cur + 5 < limit             &&
                ft_strncmp( (char*)cur, "known", 5 ) == 0 )
            break;

          T1_Skip_PS_Token( parser );
          if ( parser->root.error )
            goto Exit;
          T1_Skip_Spaces  ( parser );
          cur = parser->root.cursor;
        }

        if ( cur < limit )
        {
          T1_TokenRec  token;


           
          T1_Skip_PS_Token( parser );
          T1_ToToken( parser, &token );

           
          if ( token.type == T1_TOKEN_TYPE_ARRAY )
            cur2 = parser->root.cursor;
        }
        parser->root.cursor = cur2;
      }

       
      else if ( *cur == '/' && cur + 2 < limit )
      {
        FT_PtrDist  len;


        cur++;

        parser->root.cursor = cur;
        T1_Skip_PS_Token( parser );
        if ( parser->root.error )
          goto Exit;

        len = parser->root.cursor - cur;

        if ( len > 0 && len < 22 && parser->root.cursor < limit )
        {
          int  i;


           

           
          for ( i = 0; i < n_keywords; i++ )
          {
            T1_Field  keyword = (T1_Field)&t42_keywords[i];
            FT_Byte   *name   = (FT_Byte*)keyword->ident;


            if ( !name )
              continue;

            if ( cur[0] == name[0]                                  &&
                 len == (FT_PtrDist)ft_strlen( (const char *)name ) &&
                 ft_memcmp( cur, name, len ) == 0                   )
            {
               
              parser->root.error = t42_load_keyword( face,
                                                     loader,
                                                     keyword );
              if ( parser->root.error )
                return parser->root.error;
              break;
            }
          }
        }
      }
      else
      {
        T1_Skip_PS_Token( parser );
        if ( parser->root.error )
          goto Exit;
      }

      T1_Skip_Spaces( parser );
    }

  Exit:
    return parser->root.error;
  }
