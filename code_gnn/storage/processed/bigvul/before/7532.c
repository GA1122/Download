  sfnt_load_face( FT_Stream      stream,
                  TT_Face        face,
                  FT_Int         face_instance_index,
                  FT_Int         num_params,
                  FT_Parameter*  params )
  {
    FT_Error      error;
#ifdef TT_CONFIG_OPTION_POSTSCRIPT_NAMES
    FT_Error      psnames_error;
#endif
    FT_Bool       has_outline;
    FT_Bool       is_apple_sbit;
    FT_Bool       is_apple_sbix;
    FT_Bool       ignore_typographic_family    = FALSE;
    FT_Bool       ignore_typographic_subfamily = FALSE;

    SFNT_Service  sfnt = (SFNT_Service)face->sfnt;

    FT_UNUSED( face_instance_index );


     

    {
      FT_Int  i;


      for ( i = 0; i < num_params; i++ )
      {
        if ( params[i].tag == FT_PARAM_TAG_IGNORE_TYPOGRAPHIC_FAMILY )
          ignore_typographic_family = TRUE;
        else if ( params[i].tag == FT_PARAM_TAG_IGNORE_TYPOGRAPHIC_SUBFAMILY )
          ignore_typographic_subfamily = TRUE;
      }
    }

     

     
     
     
     
     
     
     
     
     
     
     
     
     
     

    FT_TRACE2(( "sfnt_load_face: %08p\n\n", face ));

     
#ifdef FT_CONFIG_OPTION_INCREMENTAL
    has_outline = FT_BOOL( face->root.internal->incremental_interface ||
                           tt_face_lookup_table( face, TTAG_glyf )    ||
                           tt_face_lookup_table( face, TTAG_CFF )     ||
                           tt_face_lookup_table( face, TTAG_CFF2 )    );
#else
    has_outline = FT_BOOL( tt_face_lookup_table( face, TTAG_glyf ) ||
                           tt_face_lookup_table( face, TTAG_CFF )  ||
                           tt_face_lookup_table( face, TTAG_CFF2 ) );
#endif

    is_apple_sbit = 0;
    is_apple_sbix = !face->goto_table( face, TTAG_sbix, stream, 0 );

     
    if ( is_apple_sbix )
      has_outline = FALSE;

     
     
    if ( !has_outline && sfnt->load_bhed )
    {
      LOAD_( bhed );
      is_apple_sbit = FT_BOOL( !error );
    }

     
     
    if ( !is_apple_sbit || is_apple_sbix )
    {
      LOAD_( head );
      if ( error )
        goto Exit;
    }

    if ( face->header.Units_Per_EM == 0 )
    {
      error = FT_THROW( Invalid_Table );

      goto Exit;
    }

     
     
    LOAD_( maxp );
    LOAD_( cmap );

     
     
    LOAD_( name );
    LOAD_( post );

#ifdef TT_CONFIG_OPTION_POSTSCRIPT_NAMES
    psnames_error = error;
#endif

     
     
    if ( !is_apple_sbit )
    {
       
      LOADM_( hhea, 0 );
      if ( !error )
      {
        LOADM_( hmtx, 0 );
        if ( FT_ERR_EQ( error, Table_Missing ) )
        {
          error = FT_THROW( Hmtx_Table_Missing );

#ifdef FT_CONFIG_OPTION_INCREMENTAL
           
           
          if ( face->root.internal->incremental_interface          &&
               face->root.internal->incremental_interface->funcs->
                 get_glyph_metrics                                 )
          {
            face->horizontal.number_Of_HMetrics = 0;
            error                               = FT_Err_Ok;
          }
#endif
        }
      }
      else if ( FT_ERR_EQ( error, Table_Missing ) )
      {
         
        if ( face->format_tag == TTAG_true )
        {
          FT_TRACE2(( "This is an SFNT Mac font.\n" ));

          has_outline = 0;
          error       = FT_Err_Ok;
        }
        else
        {
          error = FT_THROW( Horiz_Header_Missing );

#ifdef FT_CONFIG_OPTION_INCREMENTAL
           
           
          if ( face->root.internal->incremental_interface          &&
               face->root.internal->incremental_interface->funcs->
                 get_glyph_metrics                                 )
          {
            face->horizontal.number_Of_HMetrics = 0;
            error                               = FT_Err_Ok;
          }
#endif

        }
      }

      if ( error )
        goto Exit;

       
      LOADM_( hhea, 1 );
      if ( !error )
      {
        LOADM_( hmtx, 1 );
        if ( !error )
          face->vertical_info = 1;
      }

      if ( error && FT_ERR_NEQ( error, Table_Missing ) )
        goto Exit;

      LOAD_( os2 );
      if ( error )
      {
         
        face->os2.version = 0xFFFFU;
      }
    }

     

     
    if ( sfnt->load_eblc )
      LOAD_( eblc );

     
    LOAD_( pclt );
    LOAD_( gasp );
    LOAD_( kern );

    face->root.num_glyphs = face->max_profile.numGlyphs;

     
     
     
     
     

    face->root.family_name = NULL;
    face->root.style_name  = NULL;
    if ( face->os2.version != 0xFFFFU && face->os2.fsSelection & 256 )
    {
      if ( !ignore_typographic_family )
        GET_NAME( TYPOGRAPHIC_FAMILY, &face->root.family_name );
      if ( !face->root.family_name )
        GET_NAME( FONT_FAMILY, &face->root.family_name );

      if ( !ignore_typographic_subfamily )
        GET_NAME( TYPOGRAPHIC_SUBFAMILY, &face->root.style_name );
      if ( !face->root.style_name )
        GET_NAME( FONT_SUBFAMILY, &face->root.style_name );
    }
    else
    {
      GET_NAME( WWS_FAMILY, &face->root.family_name );
      if ( !face->root.family_name && !ignore_typographic_family )
        GET_NAME( TYPOGRAPHIC_FAMILY, &face->root.family_name );
      if ( !face->root.family_name )
        GET_NAME( FONT_FAMILY, &face->root.family_name );

      GET_NAME( WWS_SUBFAMILY, &face->root.style_name );
      if ( !face->root.style_name && !ignore_typographic_subfamily )
        GET_NAME( TYPOGRAPHIC_SUBFAMILY, &face->root.style_name );
      if ( !face->root.style_name )
        GET_NAME( FONT_SUBFAMILY, &face->root.style_name );
    }

     
    {
      FT_Face  root  = &face->root;
      FT_Long  flags = root->face_flags;


       
       
       
       
      if ( face->sbit_table_type == TT_SBIT_TABLE_TYPE_CBLC ||
           face->sbit_table_type == TT_SBIT_TABLE_TYPE_SBIX )
        flags |= FT_FACE_FLAG_COLOR;       

      if ( has_outline == TRUE )
        flags |= FT_FACE_FLAG_SCALABLE;    

       
       
      flags |= FT_FACE_FLAG_SFNT       |   
               FT_FACE_FLAG_HORIZONTAL;    

#ifdef TT_CONFIG_OPTION_POSTSCRIPT_NAMES
      if ( !psnames_error                             &&
           face->postscript.FormatType != 0x00030000L )
        flags |= FT_FACE_FLAG_GLYPH_NAMES;
#endif

       
      if ( face->postscript.isFixedPitch )
        flags |= FT_FACE_FLAG_FIXED_WIDTH;

       
      if ( face->vertical_info )
        flags |= FT_FACE_FLAG_VERTICAL;

       
      if ( TT_FACE_HAS_KERNING( face ) )
        flags |= FT_FACE_FLAG_KERNING;

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
       
       
      if ( face->variation_support & TT_FACE_FLAG_VAR_FVAR )
      {
        if ( tt_face_lookup_table( face, TTAG_glyf ) != 0 &&
             tt_face_lookup_table( face, TTAG_gvar ) != 0 )
          flags |= FT_FACE_FLAG_MULTIPLE_MASTERS;
        if ( tt_face_lookup_table( face, TTAG_CFF2 ) != 0 )
          flags |= FT_FACE_FLAG_MULTIPLE_MASTERS;
      }
#endif

      root->face_flags = flags;

       
       
       
       

      flags = 0;
      if ( has_outline == TRUE && face->os2.version != 0xFFFFU )
      {
         
         
         

        if ( face->os2.fsSelection & 512 )        
          flags |= FT_STYLE_FLAG_ITALIC;
        else if ( face->os2.fsSelection & 1 )     
          flags |= FT_STYLE_FLAG_ITALIC;

        if ( face->os2.fsSelection & 32 )         
          flags |= FT_STYLE_FLAG_BOLD;
      }
      else
      {
         

        if ( face->header.Mac_Style & 1 )
          flags |= FT_STYLE_FLAG_BOLD;

        if ( face->header.Mac_Style & 2 )
          flags |= FT_STYLE_FLAG_ITALIC;
      }

      root->style_flags |= flags;

       
       
       
       
       
       
       

      tt_face_build_cmaps( face );   


       
      {
        FT_Int  m;


        for ( m = 0; m < root->num_charmaps; m++ )
        {
          FT_CharMap  charmap = root->charmaps[m];


          charmap->encoding = sfnt_find_encoding( charmap->platform_id,
                                                  charmap->encoding_id );

#if 0
          if ( !root->charmap                           &&
               charmap->encoding == FT_ENCODING_UNICODE )
          {
             
            root->charmap = charmap;
          }
#endif
        }
      }

#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS

       
      {
        FT_UInt  count;


        count = face->sbit_num_strikes;

        if ( count > 0 )
        {
          FT_Memory        memory   = face->root.stream->memory;
          FT_UShort        em_size  = face->header.Units_Per_EM;
          FT_Short         avgwidth = face->os2.xAvgCharWidth;
          FT_Size_Metrics  metrics;

          FT_UInt*  sbit_strike_map = NULL;
          FT_UInt   strike_idx, bsize_idx;


          if ( em_size == 0 || face->os2.version == 0xFFFFU )
          {
            avgwidth = 1;
            em_size = 1;
          }

           
           
           
          if ( FT_NEW_ARRAY( root->available_sizes, count ) ||
               FT_NEW_ARRAY( sbit_strike_map, count ) )
            goto Exit;

          bsize_idx = 0;
          for ( strike_idx = 0; strike_idx < count; strike_idx++ )
          {
            FT_Bitmap_Size*  bsize = root->available_sizes + bsize_idx;


            error = sfnt->load_strike_metrics( face, strike_idx, &metrics );
            if ( error )
              continue;

            bsize->height = (FT_Short)( metrics.height >> 6 );
            bsize->width  = (FT_Short)(
              ( avgwidth * metrics.x_ppem + em_size / 2 ) / em_size );

            bsize->x_ppem = metrics.x_ppem << 6;
            bsize->y_ppem = metrics.y_ppem << 6;

             
            bsize->size   = metrics.y_ppem << 6;

             
            if ( bsize->x_ppem && bsize->y_ppem )
              sbit_strike_map[bsize_idx++] = strike_idx;
          }

           
          (void)FT_RENEW_ARRAY( sbit_strike_map, count, bsize_idx );

           
           
          if ( bsize_idx )
          {
            face->sbit_strike_map = sbit_strike_map;

            root->face_flags     |= FT_FACE_FLAG_FIXED_SIZES;
            root->num_fixed_sizes = (FT_Int)bsize_idx;
          }
        }
      }

#endif  

       
       
      if ( !FT_HAS_FIXED_SIZES( root ) && !FT_IS_SCALABLE( root ) )
        root->face_flags |= FT_FACE_FLAG_SCALABLE;


       
       
       
       
      if ( FT_IS_SCALABLE( root ) )
      {
         
         
        root->bbox.xMin    = face->header.xMin;
        root->bbox.yMin    = face->header.yMin;
        root->bbox.xMax    = face->header.xMax;
        root->bbox.yMax    = face->header.yMax;
        root->units_per_EM = face->header.Units_Per_EM;


         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         

         
         

        root->ascender  = face->horizontal.Ascender;
        root->descender = face->horizontal.Descender;

        root->height = root->ascender - root->descender +
                       face->horizontal.Line_Gap;

        if ( !( root->ascender || root->descender ) )
        {
          if ( face->os2.version != 0xFFFFU )
          {
            if ( face->os2.sTypoAscender || face->os2.sTypoDescender )
            {
              root->ascender  = face->os2.sTypoAscender;
              root->descender = face->os2.sTypoDescender;

              root->height = root->ascender - root->descender +
                             face->os2.sTypoLineGap;
            }
            else
            {
              root->ascender  =  (FT_Short)face->os2.usWinAscent;
              root->descender = -(FT_Short)face->os2.usWinDescent;

              root->height = root->ascender - root->descender;
            }
          }
        }

        root->max_advance_width  =
          (FT_Short)face->horizontal.advance_Width_Max;
        root->max_advance_height =
          (FT_Short)( face->vertical_info ? face->vertical.advance_Height_Max
                                          : root->height );

         
         
         
        root->underline_position  = face->postscript.underlinePosition -
                                    face->postscript.underlineThickness / 2;
        root->underline_thickness = face->postscript.underlineThickness;
      }

    }

  Exit:
    FT_TRACE2(( "sfnt_load_face: done\n" ));

    return error;
  }
