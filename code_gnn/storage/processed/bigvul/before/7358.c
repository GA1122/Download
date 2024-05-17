  ps_tofixedarray( FT_Byte*  *acur,
                   FT_Byte*   limit,
                   FT_Int     max_values,
                   FT_Fixed*  values,
                   FT_Int     power_ten )
  {
    FT_Byte*  cur   = *acur;
    FT_Int    count = 0;
    FT_Byte   c, ender;


    if ( cur >= limit )
      goto Exit;

     
     
    c     = *cur;
    ender = 0;

    if ( c == '[' )
      ender = ']';
    else if ( c == '{' )
      ender = '}';

    if ( ender )
      cur++;

     
    while ( cur < limit )
    {
      FT_Fixed  dummy;
      FT_Byte*  old_cur;


       
      skip_spaces( &cur, limit );
      if ( cur >= limit )
        goto Exit;

      if ( *cur == ender )
      {
        cur++;
        break;
      }

      old_cur = cur;

      if ( values && count >= max_values )
        break;

       
       
      *( values ? &values[count] : &dummy ) =
        PS_Conv_ToFixed( &cur, limit, power_ten );

      if ( old_cur == cur )
      {
        count = -1;
        goto Exit;
      }
      else
        count++;

      if ( !ender )
        break;
    }

  Exit:
    *acur = cur;
    return count;
  }
