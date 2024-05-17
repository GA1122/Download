  TT_MulFix14( FT_Int32  a,
               FT_Int    b )
  {
    FT_Int32   m, s, hi;
    FT_UInt32  l, lo;


     
    l  = (FT_UInt32)( ( a & 0xFFFFU ) * b );
    m  = ( a >> 16 ) * b;

    lo = l + (FT_UInt32)( m << 16 );
    hi = ( m >> 16 ) + ( (FT_Int32)l >> 31 ) + ( lo < l );

     
    s   = hi >> 31;
    l   = lo + (FT_UInt32)s;
    hi += s + ( l < lo );
    lo  = l;

    l   = lo + 0x2000U;
    hi += l < lo;

    return ( hi << 18 ) | ( l >> 14 );
  }
