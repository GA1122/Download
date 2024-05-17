  Ins_IP( TT_ExecContext  exc )
  {
    FT_F26Dot6  old_range, cur_range;
    FT_Vector*  orus_base;
    FT_Vector*  cur_base;
    FT_Int      twilight;


    if ( exc->top < exc->GS.loop )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      goto Fail;
    }

     
    twilight = ( exc->GS.gep0 == 0 ||
                 exc->GS.gep1 == 0 ||
                 exc->GS.gep2 == 0 );

    if ( BOUNDS( exc->GS.rp1, exc->zp0.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      goto Fail;
    }

    if ( twilight )
      orus_base = &exc->zp0.org[exc->GS.rp1];
    else
      orus_base = &exc->zp0.orus[exc->GS.rp1];

    cur_base = &exc->zp0.cur[exc->GS.rp1];

     
     
     
     
    if ( BOUNDS( exc->GS.rp1, exc->zp0.n_points ) ||
         BOUNDS( exc->GS.rp2, exc->zp1.n_points ) )
    {
      old_range = 0;
      cur_range = 0;
    }
    else
    {
      if ( twilight )
        old_range = DUALPROJ( &exc->zp1.org[exc->GS.rp2], orus_base );
      else if ( exc->metrics.x_scale == exc->metrics.y_scale )
        old_range = DUALPROJ( &exc->zp1.orus[exc->GS.rp2], orus_base );
      else
      {
        FT_Vector  vec;


        vec.x = FT_MulFix( SUB_LONG( exc->zp1.orus[exc->GS.rp2].x,
                                     orus_base->x ),
                           exc->metrics.x_scale );
        vec.y = FT_MulFix( SUB_LONG( exc->zp1.orus[exc->GS.rp2].y,
                                     orus_base->y ),
                           exc->metrics.y_scale );

        old_range = FAST_DUALPROJ( &vec );
      }

      cur_range = PROJECT( &exc->zp1.cur[exc->GS.rp2], cur_base );
    }

    for ( ; exc->GS.loop > 0; exc->GS.loop-- )
    {
      FT_UInt     point = (FT_UInt)exc->stack[--exc->args];
      FT_F26Dot6  org_dist, cur_dist, new_dist;


       
      if ( BOUNDS( point, exc->zp2.n_points ) )
      {
        if ( exc->pedantic_hinting )
        {
          exc->error = FT_THROW( Invalid_Reference );
          return;
        }
        continue;
      }

      if ( twilight )
        org_dist = DUALPROJ( &exc->zp2.org[point], orus_base );
      else if ( exc->metrics.x_scale == exc->metrics.y_scale )
        org_dist = DUALPROJ( &exc->zp2.orus[point], orus_base );
      else
      {
        FT_Vector  vec;


        vec.x = FT_MulFix( SUB_LONG( exc->zp2.orus[point].x,
                                     orus_base->x ),
                           exc->metrics.x_scale );
        vec.y = FT_MulFix( SUB_LONG( exc->zp2.orus[point].y,
                                     orus_base->y ),
                           exc->metrics.y_scale );

        org_dist = FAST_DUALPROJ( &vec );
      }

      cur_dist = PROJECT( &exc->zp2.cur[point], cur_base );

      if ( org_dist )
      {
        if ( old_range )
          new_dist = FT_MulDiv( org_dist, cur_range, old_range );
        else
        {
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           

          new_dist = org_dist;
        }
      }
      else
        new_dist = 0;

      exc->func_move( exc,
                      &exc->zp2,
                      (FT_UShort)point,
                      SUB_LONG( new_dist, cur_dist ) );
    }

  Fail:
    exc->GS.loop = 1;
    exc->new_top = exc->args;
  }
