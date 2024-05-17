  cff_operator_seac( CFF_Decoder*  decoder,
                     FT_Pos        asb,
                     FT_Pos        adx,
                     FT_Pos        ady,
                     FT_Int        bchar,
                     FT_Int        achar )
  {
    FT_Error      error;
    CFF_Builder*  builder = &decoder->builder;
    FT_Int        bchar_index, achar_index;
    TT_Face       face = decoder->builder.face;
    FT_Vector     left_bearing, advance;
    FT_Byte*      charstring;
    FT_ULong      charstring_len;
    FT_Pos        glyph_width;


    if ( decoder->seac )
    {
      FT_ERROR(( "cff_operator_seac: invalid nested seac\n" ));
      return CFF_Err_Syntax_Error;
    }

    adx += decoder->builder.left_bearing.x;
    ady += decoder->builder.left_bearing.y;

#ifdef FT_CONFIG_OPTION_INCREMENTAL
     
     
    if ( face->root.internal->incremental_interface )
    {
      bchar_index = bchar;
      achar_index = achar;
    }
    else
#endif  
    {
      CFF_Font cff = (CFF_Font)(face->extra.data);


      bchar_index = cff_lookup_glyph_by_stdcharcode( cff, bchar );
      achar_index = cff_lookup_glyph_by_stdcharcode( cff, achar );
    }

    if ( bchar_index < 0 || achar_index < 0 )
    {
      FT_ERROR(( "cff_operator_seac:"
                 " invalid seac character code arguments\n" ));
      return CFF_Err_Syntax_Error;
    }

     
     
    if ( builder->no_recurse )
    {
      FT_GlyphSlot    glyph  = (FT_GlyphSlot)builder->glyph;
      FT_GlyphLoader  loader = glyph->internal->loader;
      FT_SubGlyph     subg;


       
      error = FT_GlyphLoader_CheckSubGlyphs( loader, 2 );
      if ( error )
        goto Exit;

      subg = loader->current.subglyphs;

       
      subg->index = bchar_index;
      subg->flags = FT_SUBGLYPH_FLAG_ARGS_ARE_XY_VALUES |
                    FT_SUBGLYPH_FLAG_USE_MY_METRICS;
      subg->arg1  = 0;
      subg->arg2  = 0;
      subg++;

       
      subg->index = achar_index;
      subg->flags = FT_SUBGLYPH_FLAG_ARGS_ARE_XY_VALUES;
      subg->arg1  = (FT_Int)( adx >> 16 );
      subg->arg2  = (FT_Int)( ady >> 16 );

       
      glyph->num_subglyphs = 2;
      glyph->subglyphs     = loader->base.subglyphs;
      glyph->format        = FT_GLYPH_FORMAT_COMPOSITE;

      loader->current.num_subglyphs = 2;
    }

    FT_GlyphLoader_Prepare( builder->loader );

     
    error = cff_get_glyph_data( face, bchar_index,
                                &charstring, &charstring_len );
    if ( !error )
    {
       
      decoder->seac = TRUE;
      error = cff_decoder_parse_charstrings( decoder, charstring,
                                             charstring_len );
      decoder->seac = FALSE;

      if ( error )
        goto Exit;

      cff_free_glyph_data( face, &charstring, charstring_len );
    }

     
     

    left_bearing = builder->left_bearing;
    advance      = builder->advance;
    glyph_width  = decoder->glyph_width;

    builder->left_bearing.x = 0;
    builder->left_bearing.y = 0;

    builder->pos_x = adx - asb;
    builder->pos_y = ady;

     
    error = cff_get_glyph_data( face, achar_index,
                                &charstring, &charstring_len );
    if ( !error )
    {
       
      decoder->seac = TRUE;
      error = cff_decoder_parse_charstrings( decoder, charstring,
                                             charstring_len );
      decoder->seac = FALSE;

      if ( error )
        goto Exit;

      cff_free_glyph_data( face, &charstring, charstring_len );
    }

     
     
    builder->left_bearing = left_bearing;
    builder->advance      = advance;
    decoder->glyph_width  = glyph_width;

    builder->pos_x = 0;
    builder->pos_y = 0;

  Exit:
    return error;
  }