  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  height = metrics->height;


     
    if ( metrics->horiBearingY < 0 )
    {
      if ( height < metrics->horiBearingY )
        height = metrics->horiBearingY;
    }
    else if ( metrics->horiBearingY > 0 )
      height -= metrics->horiBearingY;

     
    if ( !advance )
      advance = height * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - height ) / 2;
    metrics->vertAdvance  = advance;
  }
