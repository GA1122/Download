  Render_Waterfall( int  first_size )
  {
    int      start_x, start_y, step_x, step_y, x, y;
    int      pt_size, max_size = 100000;
    FT_Size  size;
    FT_Face  face;

    unsigned char         text[256];
    const unsigned char*  p;


    error = FTC_Manager_LookupFace( handle->cache_manager,
                                    handle->scaler.face_id, &face );
    if ( error )
    {
       
      fprintf( stderr, "can't access font file %p\n",
               (void*)handle->scaler.face_id );
      return 0;
    }

    if ( !FT_IS_SCALABLE( face ) )
    {
      int  i;


      max_size = 0;
      for ( i = 0; i < face->num_fixed_sizes; i++ )
        if ( face->available_sizes[i].height >= max_size / 64 )
          max_size = face->available_sizes[i].height * 64;
    }

    start_x = 4;
    start_y = 3 * HEADER_HEIGHT;

    for ( pt_size = first_size; pt_size < max_size; pt_size += 64 )
    {
      sprintf( (char*)text,
               "%g: the quick brown fox jumps over the lazy dog"
               " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", pt_size / 64.0 );

      p = text;

      FTDemo_Set_Current_Charsize( handle, pt_size, status.res );

      error = FTDemo_Get_Size( handle, &size );
      if ( error )
      {
         
        continue;
      }

      step_x = ( size->metrics.max_advance >> 6 ) + 4;
      step_y = ( size->metrics.height >> 6 ) + 1;

      x = start_x;
      y = start_y + ( size->metrics.ascender >> 6 );

      start_y += step_y;

      if ( y >= display->bitmap->rows )
        break;

      while ( *p )
      {
        FT_UInt  gindex;


        gindex = FTDemo_Get_Index( handle, *p );

        error = FTDemo_Draw_Index( handle, display, gindex, &x, &y );
        if ( error )
          status.Fail++;
        else if ( X_TOO_LONG( x, size, display ) )
          break;

        p++;
      }
    }

    FTDemo_Set_Current_Charsize( handle, first_size, status.res );

    return FT_Err_Ok;
  }
