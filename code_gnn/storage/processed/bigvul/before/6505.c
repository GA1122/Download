  _bdf_atos( char*   s,
             char**  end,
             int     base )
  {
    short                 v, neg;
    const unsigned char*  dmap;


    if ( s == 0 || *s == 0 )
      return 0;

     
    switch ( base )
    {
    case 8:
      dmap = odigits;
      break;
    case 16:
      dmap = hdigits;
      break;
    default:
      base = 10;
      dmap = ddigits;
      break;
    }

     
    neg = 0;
    if ( *s == '-' )
    {
      s++;
      neg = 1;
    }

     
    if ( *s == '0'                                  &&
         ( *( s + 1 ) == 'x' || *( s + 1 ) == 'X' ) )
    {
      base = 16;
      dmap = hdigits;
      s   += 2;
    }

    for ( v = 0; sbitset( dmap, *s ); s++ )
      v = (short)( v * base + a2i[(int)*s] );

    if ( end != 0 )
      *end = s;

    return (short)( ( !neg ) ? v : -v );
  }