  Render_Stroke( int  num_indices,
                 int  first_index )
  {
    int         start_x, start_y, step_x, step_y, x, y;
    int         i;
    FT_Size     size;
    FT_Stroker  stroker = NULL;


    error = FTDemo_Get_Size( handle, &size );

    if ( error )
    {
       
      return error;
    }

    INIT_SIZE( size, start_x, start_y, step_x, step_y, x, y );

    i = first_index;

    error = FT_Stroker_New( handle->library, &stroker );
    if ( error )
      goto Exit;

    FT_Stroker_Set( stroker, 64,
                    FT_STROKER_LINECAP_ROUND,
                    FT_STROKER_LINEJOIN_ROUND,
                    0 );

    while ( i < num_indices )
    {
      int           gindex;
      FT_GlyphSlot  slot;


      if ( handle->encoding == FT_ENCODING_NONE )
        gindex = i;
      else
        gindex = FTDemo_Get_Index( handle, i );

      error = FT_Load_Glyph( size->face, gindex,
                             handle->load_flags | FT_LOAD_NO_BITMAP );
      slot = size->face->glyph;

      if ( !error && slot->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        FT_Glyph  glyph;


        error = FT_Get_Glyph( slot, &glyph );
        if ( error )
          goto Next;

        error = FT_Glyph_Stroke( &glyph, stroker, 1 );
        if ( error )
        {
          FT_Done_Glyph( glyph );
          goto Next;
        }

        error = FTDemo_Draw_Glyph( handle, display, glyph, &x, &y );
        FT_Done_Glyph( glyph );

        if ( error )
          status.Fail++;
        else if ( X_TOO_LONG( x, size, display ) )
        {
          x  = start_x;
          y += step_y;

          if ( Y_TOO_LONG( y, size, display ) )
            break;
        }
      }
      else
      {
    Next:
        status.Fail++;
      }

      i++;
    }

  Exit:
    if ( stroker )
      FT_Stroker_Done( stroker );

    return error;
  }
