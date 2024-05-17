  psh_glyph_interpolate_other_points( PSH_Glyph  glyph,
                                      FT_Int     dimension )
  {
    PSH_Dimension  dim          = &glyph->globals->dimension[dimension];
    FT_Fixed       scale        = dim->scale_mult;
    FT_Fixed       delta        = dim->scale_delta;
    PSH_Contour    contour      = glyph->contours;
    FT_UInt        num_contours = glyph->num_contours;


    for ( ; num_contours > 0; num_contours--, contour++ )
    {
      PSH_Point  start = contour->start;
      PSH_Point  first, next, point;
      FT_UInt    fit_count;


       
      next      = start + contour->count;
      fit_count = 0;
      first     = 0;

      for ( point = start; point < next; point++ )
        if ( psh_point_is_fitted( point ) )
        {
          if ( !first )
            first = point;

          fit_count++;
        }

       
       
      if ( fit_count < 2 )
      {
        if ( fit_count == 1 )
          delta = first->cur_u - FT_MulFix( first->org_u, scale );

        for ( point = start; point < next; point++ )
          if ( point != first )
            point->cur_u = FT_MulFix( point->org_u, scale ) + delta;

        goto Next_Contour;
      }

       
       
      start = first;
      do
      {
        point = first;

         
        for (;;)
        {
          next = first->next;
          if ( next == start )
            goto Next_Contour;

          if ( !psh_point_is_fitted( next ) )
            break;

          first = next;
        }

         
        for (;;)
        {
          next = next->next;
          if ( psh_point_is_fitted( next ) )
            break;
        }

         
        {
          FT_Pos    org_a, org_ab, cur_a, cur_ab;
          FT_Pos    org_c, org_ac, cur_c;
          FT_Fixed  scale_ab;


          if ( first->org_u <= next->org_u )
          {
            org_a  = first->org_u;
            cur_a  = first->cur_u;
            org_ab = next->org_u - org_a;
            cur_ab = next->cur_u - cur_a;
          }
          else
          {
            org_a  = next->org_u;
            cur_a  = next->cur_u;
            org_ab = first->org_u - org_a;
            cur_ab = first->cur_u - cur_a;
          }

          scale_ab = 0x10000L;
          if ( org_ab > 0 )
            scale_ab = FT_DivFix( cur_ab, org_ab );

          point = first->next;
          do
          {
            org_c  = point->org_u;
            org_ac = org_c - org_a;

            if ( org_ac <= 0 )
            {
               
              cur_c = cur_a + FT_MulFix( org_ac, scale );
            }
            else if ( org_ac >= org_ab )
            {
               
              cur_c = cur_a + cur_ab + FT_MulFix( org_ac - org_ab, scale );
            }
            else
            {
               
              cur_c = cur_a + FT_MulFix( org_ac, scale_ab );
            }

            point->cur_u = cur_c;

            point = point->next;

          } while ( point != next );
        }

         
        first = next;

      } while ( first != start );

    Next_Contour:
      ;
    }
  }
