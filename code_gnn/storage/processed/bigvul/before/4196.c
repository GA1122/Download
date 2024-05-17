  tt_cmap6_validate( FT_Byte*      table,
                     FT_Validator  valid )
  {
    FT_Byte*  p;
    FT_UInt   length, count;


    if ( table + 10 > valid->limit )
      FT_INVALID_TOO_SHORT;

    p      = table + 2;
    length = TT_NEXT_USHORT( p );

    p      = table + 8;              
    count  = TT_NEXT_USHORT( p );

    if ( table + length > valid->limit || length < 10 + count * 2 )
      FT_INVALID_TOO_SHORT;

     
    if ( valid->level >= FT_VALIDATE_TIGHT )
    {
      FT_UInt  gindex;


      for ( ; count > 0; count-- )
      {
        gindex = TT_NEXT_USHORT( p );
        if ( gindex >= TT_VALID_GLYPH_COUNT( valid ) )
          FT_INVALID_GLYPH_ID;
      }
    }

    return SFNT_Err_Ok;
  }