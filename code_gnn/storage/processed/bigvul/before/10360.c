  cff_lookup_glyph_by_stdcharcode( CFF_Font  cff,
                                   FT_Int    charcode )
  {
    FT_UInt    n;
    FT_UShort  glyph_sid;


     
    if ( !cff->charset.sids )
      return -1;

     
    if ( charcode < 0 || charcode > 255 )
      return -1;

     
    glyph_sid = cff_get_standard_encoding( (FT_UInt)charcode );

    for ( n = 0; n < cff->num_glyphs; n++ )
    {
      if ( cff->charset.sids[n] == glyph_sid )
        return n;
    }

    return -1;
  }
