  _bdf_is_atom( char*          line,
                unsigned long  linelen,
                char**         name,
                char**         value,
                bdf_font_t*    font )
  {
    int              hold;
    char             *sp, *ep;
    bdf_property_t*  p;


    *name = sp = ep = line;

    while ( *ep && *ep != ' ' && *ep != '\t' )
      ep++;

    hold = -1;
    if ( *ep )
    {
      hold = *ep;
      *ep  = 0;
    }

    p = bdf_get_property( sp, font );

     
    if ( hold != -1 )
      *ep = (char)hold;

     
    if ( p && p->format != BDF_ATOM )
      return 0;

     
     
    sp = ep;
    ep = line + linelen;

     
    if ( *sp )
      *sp++ = 0;
    while ( *sp                           &&
            ( *sp == ' ' || *sp == '\t' ) )
      sp++;

     
    if ( *sp == '"' )
      sp++;
    *value = sp;

     
    while ( ep > sp                                       &&
            ( *( ep - 1 ) == ' ' || *( ep - 1 ) == '\t' ) )
      *--ep = 0;

     
    if ( ep > sp && *( ep - 1 ) == '"' )
      *--ep = 0;

    return 1;
  }
