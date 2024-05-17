  cff_get_standard_encoding( FT_UInt  charcode )
  {
    return (FT_UShort)( charcode < 256 ? cff_standard_encoding[charcode]
                                       : 0 );
  }
