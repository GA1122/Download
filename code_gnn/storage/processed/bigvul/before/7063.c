  T1_Open_Face( T1_Face  face )
  {
    T1_LoaderRec   loader;
    T1_Parser      parser;
    T1_Font        type1 = &face->type1;
    PS_Private     priv  = &type1->private_dict;
    FT_Error       error;

    PSAux_Service  psaux = (PSAux_Service)face->psaux;


    t1_init_loader( &loader, face );

     
    face->ndv_idx          = -1;
    face->cdv_idx          = -1;
    face->len_buildchar    = 0;

    priv->blue_shift       = 7;
    priv->blue_fuzz        = 1;
    priv->lenIV            = 4;
    priv->expansion_factor = (FT_Fixed)( 0.06 * 0x10000L );
    priv->blue_scale       = (FT_Fixed)( 0.039625 * 0x10000L * 1000 );

    parser = &loader.parser;
    error  = T1_New_Parser( parser,
                            face->root.stream,
                            face->root.memory,
                            psaux );
    if ( error )
      goto Exit;

    error = parse_dict( face, &loader,
                        parser->base_dict, parser->base_len );
    if ( error )
      goto Exit;

    error = T1_Get_Private_Dict( parser, psaux );
    if ( error )
      goto Exit;

    error = parse_dict( face, &loader,
                        parser->private_dict, parser->private_len );
    if ( error )
      goto Exit;

     
    priv->num_blue_values &= ~1;

#ifndef T1_CONFIG_OPTION_NO_MM_SUPPORT

    if ( face->blend                                                     &&
         face->blend->num_default_design_vector != 0                     &&
         face->blend->num_default_design_vector != face->blend->num_axis )
    {
       
      FT_ERROR(( "T1_Open_Face(): /DesignVector contains %u entries "
                 "while there are %u axes.\n",
                 face->blend->num_default_design_vector,
                 face->blend->num_axis ));

      face->blend->num_default_design_vector = 0;
    }

     
     
    if ( face->blend                                             &&
         ( !face->blend->num_designs || !face->blend->num_axis ) )
      T1_Done_Blend( face );

     
    if ( face->blend )
    {
      FT_UInt  i;


      for ( i = 0; i < face->blend->num_axis; i++ )
        if ( !face->blend->design_map[i].num_points )
        {
          T1_Done_Blend( face );
          break;
        }
    }

    if ( face->blend )
    {
      if ( face->len_buildchar > 0 )
      {
        FT_Memory  memory = face->root.memory;


        if ( FT_NEW_ARRAY( face->buildchar, face->len_buildchar ) )
        {
          FT_ERROR(( "T1_Open_Face: cannot allocate BuildCharArray\n" ));
          face->len_buildchar = 0;
          goto Exit;
        }
      }
    }
    else
      face->len_buildchar = 0;

#endif  

     
     
    type1->num_glyphs = loader.num_glyphs;

    if ( loader.subrs.init )
    {
      loader.subrs.init  = 0;
      type1->num_subrs   = loader.num_subrs;
      type1->subrs_block = loader.subrs.block;
      type1->subrs       = loader.subrs.elements;
      type1->subrs_len   = loader.subrs.lengths;
    }

    if ( !IS_INCREMENTAL )
      if ( !loader.charstrings.init )
      {
        FT_ERROR(( "T1_Open_Face: no `/CharStrings' array in face\n" ));
        error = FT_THROW( Invalid_File_Format );
      }

    loader.charstrings.init  = 0;
    type1->charstrings_block = loader.charstrings.block;
    type1->charstrings       = loader.charstrings.elements;
    type1->charstrings_len   = loader.charstrings.lengths;

     
     
    type1->glyph_names_block    = loader.glyph_names.block;
    type1->glyph_names          = (FT_String**)loader.glyph_names.elements;
    loader.glyph_names.block    = 0;
    loader.glyph_names.elements = 0;

     
    if ( type1->encoding_type == T1_ENCODING_TYPE_ARRAY )
    {
      FT_Int    charcode, idx, min_char, max_char;
      FT_Byte*  glyph_name;


       
       
       
       

      min_char = 0;
      max_char = 0;

      charcode = 0;
      for ( ; charcode < loader.encoding_table.max_elems; charcode++ )
      {
        FT_Byte*  char_name;


        type1->encoding.char_index[charcode] = 0;
        type1->encoding.char_name [charcode] = (char *)".notdef";

        char_name = loader.encoding_table.elements[charcode];
        if ( char_name )
          for ( idx = 0; idx < type1->num_glyphs; idx++ )
          {
            glyph_name = (FT_Byte*)type1->glyph_names[idx];
            if ( ft_strcmp( (const char*)char_name,
                            (const char*)glyph_name ) == 0 )
            {
              type1->encoding.char_index[charcode] = (FT_UShort)idx;
              type1->encoding.char_name [charcode] = (char*)glyph_name;

               
               
              if ( ft_strcmp( (const char*)".notdef",
                              (const char*)glyph_name ) != 0 )
              {
                if ( charcode < min_char )
                  min_char = charcode;
                if ( charcode >= max_char )
                  max_char = charcode + 1;
              }
              break;
            }
          }
      }

      type1->encoding.code_first = min_char;
      type1->encoding.code_last  = max_char;
      type1->encoding.num_chars  = loader.num_chars;
    }

  Exit:
    t1_done_loader( &loader );
    return error;
  }
