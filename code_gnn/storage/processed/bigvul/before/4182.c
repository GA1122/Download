  tt_cmap2_get_subheader( FT_Byte*   table,
                          FT_UInt32  char_code )
  {
    FT_Byte*  result = NULL;


    if ( char_code < 0x10000UL )
    {
      FT_UInt   char_lo = (FT_UInt)( char_code & 0xFF );
      FT_UInt   char_hi = (FT_UInt)( char_code >> 8 );
      FT_Byte*  p       = table + 6;     
      FT_Byte*  subs    = table + 518;   
      FT_Byte*  sub;


      if ( char_hi == 0 )
      {
         
         
         
        sub = subs;   

         
         
         
         
        p += char_lo * 2;
        if ( TT_PEEK_USHORT( p ) != 0 )
          goto Exit;
      }
      else
      {
         

         
        p  += char_hi * 2;
         
        sub = subs + ( FT_PAD_FLOOR( TT_PEEK_USHORT( p ), 8 ) );

         
        if ( sub == subs )
          goto Exit;
      }
      result = sub;
    }
  Exit:
    return result;
  }
