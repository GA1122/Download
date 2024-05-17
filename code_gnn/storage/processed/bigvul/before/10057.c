  Render_Text( int  num_indices,
               int  first_index )
  {
    int      start_x, start_y, step_x, step_y, x, y;
    int      i;
    FT_Size  size;

    const char*  p;
    const char*  pEnd;


    num_indices = num_indices;   

    error = FTDemo_Get_Size( handle, &size );
    if ( error )
    {
       
      return error;
    }

    INIT_SIZE( size, start_x, start_y, step_x, step_y, x, y );

    i = first_index;

    p    = (const char*)Text;
    pEnd = p + strlen( (const char*)Text );

    while ( i > 0 )
    {
      utf8_next( &p, pEnd );
      i--;
    }

    while ( num_indices != 0 )
    {
      FT_UInt  gindex;
      int      ch;


      ch = utf8_next( &p, pEnd );
      if ( ch < 0 )
        break;

      gindex = FTDemo_Get_Index( handle, ch );

      error = FTDemo_Draw_Index( handle, display, gindex, &x, &y );
      if ( error )
        status.Fail++;
      else
      {
         
        x--;

        if ( X_TOO_LONG( x, size, display ) )
        {
          x  = start_x;
          y += step_y;

          if ( Y_TOO_LONG( y, size, display ) )
            break;
        }
      }

      if ( num_indices > 0 )
        num_indices -= 1;
    }

    return FT_Err_Ok;
  }
