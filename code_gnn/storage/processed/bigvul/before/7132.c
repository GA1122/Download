  _bdf_list_split( _bdf_list_t*   list,
                   char*          separators,
                   char*          line,
                   unsigned long  linelen )
  {
    int       mult, final_empty;
    char      *sp, *ep, *end;
    char      seps[32];
    FT_Error  error = FT_Err_Ok;


     
    list->used = 0;
    if ( list->size )
    {
      list->field[0] = (char*)empty;
      list->field[1] = (char*)empty;
      list->field[2] = (char*)empty;
      list->field[3] = (char*)empty;
      list->field[4] = (char*)empty;
    }

     
    if ( linelen == 0 || line[0] == 0 )
      goto Exit;

     
     
     
    if ( separators == 0 || *separators == 0 )
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }

     
    FT_MEM_ZERO( seps, 32 );

     
     
     
    for ( mult = 0, sp = separators; sp && *sp; sp++ )
    {
      if ( *sp == '+' && *( sp + 1 ) == 0 )
        mult = 1;
      else
        setsbit( seps, *sp );
    }

     
    for ( final_empty = 0, sp = ep = line, end = sp + linelen;
          sp < end && *sp; )
    {
       
      for ( ; *ep && !sbitset( seps, *ep ); ep++ )
        ;

       
      if ( list->used == list->size )
      {
        error = _bdf_list_ensure( list, list->used + 1 );
        if ( error )
          goto Exit;
      }

       
      list->field[list->used++] = ( ep > sp ) ? sp : (char*)empty;

      sp = ep;

      if ( mult )
      {
         
         
        for ( ; *ep && sbitset( seps, *ep ); ep++ )
          *ep = 0;
      }
      else if ( *ep != 0 )
         
         
        *ep++ = 0;

      final_empty = ( ep > sp && *ep == 0 );
      sp = ep;
    }

     
    if ( list->used + final_empty >= list->size )
    {
      error = _bdf_list_ensure( list, list->used + final_empty + 1 );
      if ( error )
        goto Exit;
    }

    if ( final_empty )
      list->field[list->used++] = (char*)empty;

    list->field[list->used] = 0;

  Exit:
    return error;
  }
