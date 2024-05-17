  t42_is_space( FT_Byte  c )
  {
    return ( c == ' '  || c == '\t'              ||
             c == '\r' || c == '\n' || c == '\f' ||
             c == '\0'                           );
  }
