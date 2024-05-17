  Render_Slanted( int  num_indices,
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
        FT_Matrix    shear;
        FT_Outline*  outline;


         
         
         
         
         
         
         
         
         
         
         
         
         

        shear.xx = 1 << 16;
        shear.xy = (FT_Fixed)( status.slant * ( 1 << 16 ) );
        shear.yx = 0;
        shear.yy = 1 << 16;

        outline = &(face->glyph)->outline;

        FT_Outline_Transform( outline, &shear );

        error = FTDemo_Draw_Slot( handle, display, face->glyph, &x, &y );

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
        status.Fail++;

      i++;
    }

    return error;
  }
