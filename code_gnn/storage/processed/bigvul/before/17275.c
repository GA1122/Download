  tt_cmap6_char_index( TT_CMap    cmap,
                       FT_UInt32  char_code )
  {
    FT_Byte*  table  = cmap->data;
    FT_UInt   result = 0;
    FT_Byte*  p      = table + 6;
    FT_UInt   start  = TT_NEXT_USHORT( p );
    FT_UInt   count  = TT_NEXT_USHORT( p );
    FT_UInt   idx    = (FT_UInt)( char_code - start );


    if ( idx < count )
    {
      p += 2 * idx;
      result = TT_PEEK_USHORT( p );
    }

    return result;
  }
