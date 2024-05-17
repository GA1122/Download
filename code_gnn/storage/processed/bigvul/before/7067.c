  parse_subrs( T1_Face    face,
               T1_Loader  loader )
  {
    T1_Parser  parser = &loader->parser;
    PS_Table   table  = &loader->subrs;
    FT_Memory  memory = parser->root.memory;
    FT_Error   error;
    FT_Int     num_subrs;

    PSAux_Service  psaux = (PSAux_Service)face->psaux;


    T1_Skip_Spaces( parser );

     
    if ( parser->root.cursor < parser->root.limit &&
         *parser->root.cursor == '['              )
    {
      T1_Skip_PS_Token( parser );
      T1_Skip_Spaces  ( parser );
      if ( parser->root.cursor >= parser->root.limit ||
           *parser->root.cursor != ']'               )
        parser->root.error = FT_THROW( Invalid_File_Format );
      return;
    }

    num_subrs = (FT_Int)T1_ToInt( parser );

     
    T1_Skip_PS_Token( parser );          
    if ( parser->root.error )
      return;
    T1_Skip_Spaces( parser );

     
     
    if ( !loader->num_subrs )
    {
      error = psaux->ps_table_funcs->init( table, num_subrs, memory );
      if ( error )
        goto Fail;
    }

     
     
     
     
    for (;;)
    {
      FT_Long   idx, size;
      FT_Byte*  base;


       
       
      if ( parser->root.cursor + 4 >= parser->root.limit          ||
          ft_strncmp( (char*)parser->root.cursor, "dup", 3 ) != 0 )
        break;

      T1_Skip_PS_Token( parser );        

      idx = T1_ToInt( parser );

      if ( !read_binary_data( parser, &size, &base, IS_INCREMENTAL ) )
        return;

       
       
       
       
      T1_Skip_PS_Token( parser );    
      if ( parser->root.error )
        return;
      T1_Skip_Spaces  ( parser );

      if ( parser->root.cursor + 4 < parser->root.limit            &&
           ft_strncmp( (char*)parser->root.cursor, "put", 3 ) == 0 )
      {
        T1_Skip_PS_Token( parser );  
        T1_Skip_Spaces  ( parser );
      }

       
      if ( loader->num_subrs )
        continue;

       
       
       
       
       
      if ( face->type1.private_dict.lenIV >= 0 )
      {
        FT_Byte*  temp;


         
         
         
        if ( size < face->type1.private_dict.lenIV )
        {
          error = FT_THROW( Invalid_File_Format );
          goto Fail;
        }

         
        if ( FT_ALLOC( temp, size ) )
          goto Fail;
        FT_MEM_COPY( temp, base, size );
        psaux->t1_decrypt( temp, size, 4330 );
        size -= face->type1.private_dict.lenIV;
        error = T1_Add_Table( table, (FT_Int)idx,
                              temp + face->type1.private_dict.lenIV, size );
        FT_FREE( temp );
      }
      else
        error = T1_Add_Table( table, (FT_Int)idx, base, size );
      if ( error )
        goto Fail;
    }

    if ( !loader->num_subrs )
      loader->num_subrs = num_subrs;

    return;

  Fail:
    parser->root.error = error;
  }
