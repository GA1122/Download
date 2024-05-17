  ft_gray_for_premultiplied_srgb_bgra( const FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


     
    if ( !a )
      return 0;

     

    l = (  4732UL   * bgra[0] * bgra[0] +
          46871UL   * bgra[1] * bgra[1] +
          13933UL   * bgra[2] * bgra[2] ) >> 16;

     

    return (FT_Byte)( a - l / a );
  }
