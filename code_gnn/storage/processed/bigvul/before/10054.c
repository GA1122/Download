  Render_Embolden( int  num_indices,
                   int  first_index )
  {
    int      start_x, start_y, step_x, step_y, x, y;
    int      i;
    FT_Size  size;


    error = FTDemo_Get_Size( handle, &size );

    if ( error )
    {
       
      return error;
    }

    INIT_SIZE( size, start_x, start_y, step_x, step_y, x, y );

    i = first_index;

    while ( i < num_indices )
    {
      int      gindex;
      FT_Face  face = size->face;


      if ( handle->encoding == FT_ENCODING_NONE )
        gindex = i;
      else
        gindex = FTDemo_Get_Index( handle, i );

      error = FT_Load_Glyph( face, gindex, handle->load_flags );
      if ( !error )
      {
         
         

        FT_GlyphSlot  slot    = face->glyph;
        FT_Library    library = slot->library;
        FT_Pos        xstr, ystr;


        if ( slot->format != FT_GLYPH_FORMAT_OUTLINE &&
             slot->format != FT_GLYPH_FORMAT_BITMAP )
          goto Next;

        xstr = FT_MulFix( face->units_per_EM,
                          face->size->metrics.y_scale );
        xstr = (FT_Fixed)( xstr * status.bold_factor );
        ystr = xstr;

        if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          error = FT_Outline_Embolden( &slot->outline, xstr );
           

          xstr = xstr * 2;
          ystr = xstr;
        }
        else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
        {
           
          xstr &= ~63;
          ystr &= ~63;

          error = FT_GlyphSlot_Own_Bitmap( slot );
          if ( error )
            goto Next;

          error = FT_Bitmap_Embolden( library, &slot->bitmap, xstr, ystr );
          if ( error )
            goto Next;
        }

        if ( slot->advance.x )
          slot->advance.x += xstr;

        if ( slot->advance.y )
          slot->advance.y += ystr;

        slot->metrics.width        += xstr;
        slot->metrics.height       += ystr;
        slot->metrics.horiBearingY += ystr;
        slot->metrics.horiAdvance  += xstr;
        slot->metrics.vertBearingX -= xstr / 2;
        slot->metrics.vertBearingY += ystr;
        slot->metrics.vertAdvance  += ystr;

        if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
          slot->bitmap_top += ystr >> 6;

        error = FTDemo_Draw_Slot( handle, display, slot, &x, &y );

        if ( error )
          goto Next;
        else if ( X_TOO_LONG( x, size, display ) )
        {
          x  = start_x;
          y += step_y;

          if ( Y_TOO_LONG( y, size, display ) )
            break;
        }
      }
      else
    Next:
        status.Fail++;

      i++;
    }

    return error;
  }
