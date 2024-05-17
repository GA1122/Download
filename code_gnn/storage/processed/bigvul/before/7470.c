  t1operator_seac( T1_Decoder  decoder,
                   FT_Pos      asb,
                   FT_Pos      adx,
                   FT_Pos      ady,
                   FT_Int      bchar,
                   FT_Int      achar )
  {
    FT_Error     error;
    FT_Int       bchar_index, achar_index;
#if 0
    FT_Int       n_base_points;
    FT_Outline*  base = decoder->builder.base;
#endif
    FT_Vector    left_bearing, advance;

#ifdef FT_CONFIG_OPTION_INCREMENTAL
    T1_Face      face  = (T1_Face)decoder->builder.face;
#endif


    if ( decoder->seac )
    {
      FT_ERROR(( "t1operator_seac: invalid nested seac\n" ));
      return FT_THROW( Syntax_Error );
    }

    if ( decoder->builder.metrics_only )
    {
      FT_ERROR(( "t1operator_seac: unexpected seac\n" ));
      return FT_THROW( Syntax_Error );
    }

     
    adx += decoder->builder.left_bearing.x;

     
     
#ifdef FT_CONFIG_OPTION_INCREMENTAL
    if ( decoder->glyph_names == 0                   &&
         !face->root.internal->incremental_interface )
#else
    if ( decoder->glyph_names == 0 )
#endif  
    {
      FT_ERROR(( "t1operator_seac:"
                 " glyph names table not available in this font\n" ));
      return FT_THROW( Syntax_Error );
    }

#ifdef FT_CONFIG_OPTION_INCREMENTAL
    if ( face->root.internal->incremental_interface )
    {
       
      bchar_index = bchar;
      achar_index = achar;
    }
    else
#endif
    {
      bchar_index = t1_lookup_glyph_by_stdcharcode( decoder, bchar );
      achar_index = t1_lookup_glyph_by_stdcharcode( decoder, achar );
    }

    if ( bchar_index < 0 || achar_index < 0 )
    {
      FT_ERROR(( "t1operator_seac:"
                 " invalid seac character code arguments\n" ));
      return FT_THROW( Syntax_Error );
    }

     
     
    if ( decoder->builder.no_recurse )
    {
      FT_GlyphSlot    glyph  = (FT_GlyphSlot)decoder->builder.glyph;
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
      subg->arg1  = (FT_Int)FIXED_TO_INT( adx - asb );
      subg->arg2  = (FT_Int)FIXED_TO_INT( ady );

       
      glyph->num_subglyphs = 2;
      glyph->subglyphs     = loader->base.subglyphs;
      glyph->format        = FT_GLYPH_FORMAT_COMPOSITE;

      loader->current.num_subglyphs = 2;
      goto Exit;
    }

     
     

    FT_GlyphLoader_Prepare( decoder->builder.loader );   

     
    decoder->seac = TRUE;
    error = t1_decoder_parse_glyph( decoder, (FT_UInt)bchar_index );
    decoder->seac = FALSE;
    if ( error )
      goto Exit;

     
     

    left_bearing = decoder->builder.left_bearing;
    advance      = decoder->builder.advance;

    decoder->builder.left_bearing.x = 0;
    decoder->builder.left_bearing.y = 0;

    decoder->builder.pos_x = adx - asb;
    decoder->builder.pos_y = ady;

     
     

     
    decoder->seac = TRUE;
    error = t1_decoder_parse_glyph( decoder, (FT_UInt)achar_index );
    decoder->seac = FALSE;
    if ( error )
      goto Exit;

     
     

    decoder->builder.left_bearing = left_bearing;
    decoder->builder.advance      = advance;

    decoder->builder.pos_x = 0;
    decoder->builder.pos_y = 0;

  Exit:
    return error;
  }
