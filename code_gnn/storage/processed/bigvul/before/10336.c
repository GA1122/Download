  psh_hint_align( PSH_Hint     hint,
                  PSH_Globals  globals,
                  FT_Int       dimension,
                  PSH_Glyph    glyph )
  {
    PSH_Dimension  dim   = &globals->dimension[dimension];
    FT_Fixed       scale = dim->scale_mult;
    FT_Fixed       delta = dim->scale_delta;


    if ( !psh_hint_is_fitted( hint ) )
    {
      FT_Pos  pos = FT_MulFix( hint->org_pos, scale ) + delta;
      FT_Pos  len = FT_MulFix( hint->org_len, scale );

      FT_Int            do_snapping;
      FT_Pos            fit_len;
      PSH_AlignmentRec  align;


       
      if ( ( dimension == 0 && !glyph->do_horz_hints ) ||
           ( dimension == 1 && !glyph->do_vert_hints ) )
      {
        hint->cur_pos = pos;
        hint->cur_len = len;

        psh_hint_set_fitted( hint );
        return;
      }

       
      do_snapping = ( dimension == 0 && glyph->do_horz_snapping ) ||
                    ( dimension == 1 && glyph->do_vert_snapping );

      hint->cur_len = fit_len = len;

       
      align.align     = PSH_BLUE_ALIGN_NONE;
      align.align_bot = align.align_top = 0;

      if ( dimension == 1 )
        psh_blues_snap_stem( &globals->blues,
                             hint->org_pos + hint->org_len,
                             hint->org_pos,
                             &align );

      switch ( align.align )
      {
      case PSH_BLUE_ALIGN_TOP:
         
        hint->cur_pos = align.align_top - fit_len;
        break;

      case PSH_BLUE_ALIGN_BOT:
         
        hint->cur_pos = align.align_bot;
        break;

      case PSH_BLUE_ALIGN_TOP | PSH_BLUE_ALIGN_BOT:
         
        hint->cur_pos = align.align_bot;
        hint->cur_len = align.align_top - align.align_bot;
        break;

      default:
        {
          PSH_Hint  parent = hint->parent;


          if ( parent )
          {
            FT_Pos  par_org_center, par_cur_center;
            FT_Pos  cur_org_center, cur_delta;


             
            if ( !psh_hint_is_fitted( parent ) )
              psh_hint_align( parent, globals, dimension, glyph );

             
             
             
            par_org_center = parent->org_pos + ( parent->org_len >> 1 );
            par_cur_center = parent->cur_pos + ( parent->cur_len >> 1 );
            cur_org_center = hint->org_pos   + ( hint->org_len   >> 1 );

            cur_delta = FT_MulFix( cur_org_center - par_org_center, scale );
            pos       = par_cur_center + cur_delta - ( len >> 1 );
          }

          hint->cur_pos = pos;
          hint->cur_len = fit_len;

           
          if ( glyph->do_stem_adjust )
          {
            if ( len <= 64 )
            {
               
              if ( len >= 32 )
              {
                 
                pos = FT_PIX_FLOOR( pos + ( len >> 1 ) );
                len = 64;
              }
              else if ( len > 0 )
              {
                 
                FT_Pos  left_nearest  = FT_PIX_ROUND( pos );
                FT_Pos  right_nearest = FT_PIX_ROUND( pos + len );
                FT_Pos  left_disp     = left_nearest - pos;
                FT_Pos  right_disp    = right_nearest - ( pos + len );


                if ( left_disp < 0 )
                  left_disp = -left_disp;
                if ( right_disp < 0 )
                  right_disp = -right_disp;
                if ( left_disp <= right_disp )
                  pos = left_nearest;
                else
                  pos = right_nearest;
              }
              else
              {
                 
                pos = FT_PIX_ROUND( pos );
              }
            }
            else
            {
              len = psh_dimension_quantize_len( dim, len, 0 );
            }
          }

           
           
          hint->cur_pos = pos + psh_hint_snap_stem_side_delta( pos, len );
          hint->cur_len = len;
        }
      }

      if ( do_snapping )
      {
        pos = hint->cur_pos;
        len = hint->cur_len;

        if ( len < 64 )
          len = 64;
        else
          len = FT_PIX_ROUND( len );

        switch ( align.align )
        {
          case PSH_BLUE_ALIGN_TOP:
            hint->cur_pos = align.align_top - len;
            hint->cur_len = len;
            break;

          case PSH_BLUE_ALIGN_BOT:
            hint->cur_len = len;
            break;

          case PSH_BLUE_ALIGN_BOT | PSH_BLUE_ALIGN_TOP:
             
            break;


          default:
            hint->cur_len = len;
            if ( len & 64 )
              pos = FT_PIX_FLOOR( pos + ( len >> 1 ) ) + 32;
            else
              pos = FT_PIX_ROUND( pos + ( len >> 1 ) );

            hint->cur_pos = pos - ( len >> 1 );
            hint->cur_len = len;
        }
      }

      psh_hint_set_fitted( hint );

#ifdef DEBUG_HINTER
      if ( ps_debug_hint_func )
        ps_debug_hint_func( hint, dimension );
#endif
    }
  }
