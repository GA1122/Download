  ft_glyphslot_grid_fit_metrics( FT_GlyphSlot  slot,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &slot->metrics;
    FT_Pos             right, bottom;


    if ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL ( metrics->horiBearingY );

      right  = FT_PIX_CEIL( metrics->vertBearingX + metrics->width );
      bottom = FT_PIX_CEIL( metrics->vertBearingY + metrics->height );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->width  = right - metrics->vertBearingX;
      metrics->height = bottom - metrics->vertBearingY;
    }
    else
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      right  = FT_PIX_CEIL ( metrics->horiBearingX + metrics->width );
      bottom = FT_PIX_FLOOR( metrics->horiBearingY - metrics->height );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL ( metrics->horiBearingY );

      metrics->width  = right - metrics->horiBearingX;
      metrics->height = metrics->horiBearingY - bottom;
    }

    metrics->horiAdvance = FT_PIX_ROUND( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_ROUND( metrics->vertAdvance );
  }
