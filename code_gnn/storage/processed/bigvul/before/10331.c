  psh_glyph_init( PSH_Glyph    glyph,
                  FT_Outline*  outline,
                  PS_Hints     ps_hints,
                  PSH_Globals  globals )
  {
    FT_Error   error;
    FT_Memory  memory;


     
    FT_MEM_ZERO( glyph, sizeof ( *glyph ) );

    memory = glyph->memory = globals->memory;

     
    if ( FT_NEW_ARRAY( glyph->points,   outline->n_points   ) ||
         FT_NEW_ARRAY( glyph->contours, outline->n_contours ) )
      goto Exit;

    glyph->num_points   = outline->n_points;
    glyph->num_contours = outline->n_contours;

    {
      FT_UInt      first = 0, next, n;
      PSH_Point    points  = glyph->points;
      PSH_Contour  contour = glyph->contours;


      for ( n = 0; n < glyph->num_contours; n++ )
      {
        FT_Int     count;
        PSH_Point  point;


        next  = outline->contours[n] + 1;
        count = next - first;

        contour->start = points + first;
        contour->count = (FT_UInt)count;

        if ( count > 0 )
        {
          point = points + first;

          point->prev    = points + next - 1;
          point->contour = contour;

          for ( ; count > 1; count-- )
          {
            point[0].next = point + 1;
            point[1].prev = point;
            point++;
            point->contour = contour;
          }
          point->next = points + first;
        }

        contour++;
        first = next;
      }
    }

    {
      PSH_Point   points = glyph->points;
      PSH_Point   point  = points;
      FT_Vector*  vec    = outline->points;
      FT_UInt     n;


      for ( n = 0; n < glyph->num_points; n++, point++ )
      {
        FT_Int  n_prev = (FT_Int)( point->prev - points );
        FT_Int  n_next = (FT_Int)( point->next - points );
        FT_Pos  dxi, dyi, dxo, dyo;


        if ( !( outline->tags[n] & FT_CURVE_TAG_ON ) )
          point->flags = PSH_POINT_OFF;

        dxi = vec[n].x - vec[n_prev].x;
        dyi = vec[n].y - vec[n_prev].y;

        point->dir_in = (FT_Char)psh_compute_dir( dxi, dyi );

        dxo = vec[n_next].x - vec[n].x;
        dyo = vec[n_next].y - vec[n].y;

        point->dir_out = (FT_Char)psh_compute_dir( dxo, dyo );

         
        if ( point->flags & PSH_POINT_OFF )
          point->flags |= PSH_POINT_SMOOTH;

        else if ( point->dir_in == point->dir_out )
        {
          if ( point->dir_out != PSH_DIR_NONE           ||
               psh_corner_is_flat( dxi, dyi, dxo, dyo ) )
            point->flags |= PSH_POINT_SMOOTH;
        }
      }
    }

    glyph->outline = outline;
    glyph->globals = globals;

#ifdef COMPUTE_INFLEXS
    psh_glyph_load_points( glyph, 0 );
    psh_glyph_compute_inflections( glyph );
#endif  

     
    error = psh_hint_table_init( &glyph->hint_tables [0],
                                 &ps_hints->dimension[0].hints,
                                 &ps_hints->dimension[0].masks,
                                 &ps_hints->dimension[0].counters,
                                 memory );
    if ( error )
      goto Exit;

    error = psh_hint_table_init( &glyph->hint_tables [1],
                                 &ps_hints->dimension[1].hints,
                                 &ps_hints->dimension[1].masks,
                                 &ps_hints->dimension[1].counters,
                                 memory );
    if ( error )
      goto Exit;

  Exit:
    return error;
  }
