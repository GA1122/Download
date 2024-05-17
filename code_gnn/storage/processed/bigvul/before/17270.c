  tt_cmap2_validate( FT_Byte*      table,
                     FT_Validator  valid )
  {
    FT_Byte*  p;
    FT_UInt   length;

    FT_UInt   n, max_subs;
    FT_Byte*  keys;         
    FT_Byte*  subs;         
    FT_Byte*  glyph_ids;    


    if ( table + 2 + 2 > valid->limit )
      FT_INVALID_TOO_SHORT;

    p      = table + 2;            
    length = TT_NEXT_USHORT( p );

    if ( table + length > valid->limit || length < 6 + 512 )
      FT_INVALID_TOO_SHORT;

    keys = table + 6;

     
    p        = keys;
    max_subs = 0;
    for ( n = 0; n < 256; n++ )
    {
      FT_UInt  idx = TT_NEXT_USHORT( p );


       
      if ( valid->level >= FT_VALIDATE_PARANOID && ( idx & 7 ) != 0 )
        FT_INVALID_DATA;

      idx >>= 3;

      if ( idx > max_subs )
        max_subs = idx;
    }

    FT_ASSERT( p == table + 518 );

    subs      = p;
    glyph_ids = subs + ( max_subs + 1 ) * 8;
    if ( glyph_ids > valid->limit )
      FT_INVALID_TOO_SHORT;

     
    for ( n = 0; n <= max_subs; n++ )
    {
      FT_UInt  first_code, code_count, offset;
      FT_Int   delta;


      first_code = TT_NEXT_USHORT( p );
      code_count = TT_NEXT_USHORT( p );
      delta      = TT_NEXT_SHORT( p );
      offset     = TT_NEXT_USHORT( p );

       
      if ( code_count == 0 )
        continue;

       
      if ( valid->level >= FT_VALIDATE_PARANOID )
      {
        if ( first_code >= 256 || first_code + code_count > 256 )
          FT_INVALID_DATA;
      }

       
      if ( offset != 0 )
      {
        FT_Byte*  ids;


        ids = p - 2 + offset;
        if ( ids < glyph_ids || ids + code_count * 2 > table + length )
          FT_INVALID_OFFSET;

         
        if ( valid->level >= FT_VALIDATE_TIGHT )
        {
          FT_Byte*  limit = p + code_count * 2;
          FT_UInt   idx;


          for ( ; p < limit; )
          {
            idx = TT_NEXT_USHORT( p );
            if ( idx != 0 )
            {
              idx = (FT_UInt)( (FT_Int)idx + delta ) & 0xFFFFU;
              if ( idx >= TT_VALID_GLYPH_COUNT( valid ) )
                FT_INVALID_GLYPH_ID;
            }
          }
        }
      }
    }

    return FT_Err_Ok;
  }