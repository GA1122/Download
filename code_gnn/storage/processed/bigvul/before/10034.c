  Render_All( int  first_glyph,
              int  pt_size )
  {
    FT_F26Dot6  start_x, start_y, step_x, step_y, x, y;
    int         i;


    start_x = 4;
    start_y = 36 + pt_size;

    step_x = size->metrics.x_ppem + 4;
    step_y = size->metrics.y_ppem + 10;

    x = start_x;
    y = start_y;

    i = first_glyph;

#if 0
     while ( i < first_glyph + 1 )
#else
     while ( i < num_glyphs )
#endif
    {
      if ( !( error = LoadChar( i, hinted ) ) )
      {
#ifdef DEBUG
        if ( i <= first_glyph + 6 )
        {
          LOG(( "metrics[%02d] = [%x %x]\n",
                i,
                glyph->metrics.horiBearingX,
                glyph->metrics.horiAdvance ));

          if ( i == first_glyph + 6 )
            LOG(( "-------------------------\n" ));
        }
#endif

        Render_Glyph( x, y );

        x += ( glyph->metrics.horiAdvance >> 6 ) + 1;

        if ( x + size->metrics.x_ppem > bit.width )
        {
          x  = start_x;
          y += step_y;

          if ( y >= bit.rows )
            return FT_Err_Ok;
        }
      }
      else
        Fail++;

      i++;
    }

    return FT_Err_Ok;
  }
