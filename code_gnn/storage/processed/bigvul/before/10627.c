  Ins_MIAP( TT_ExecContext  exc,
            FT_Long*        args )
  {
    FT_ULong    cvtEntry;
    FT_UShort   point;
    FT_F26Dot6  distance;
    FT_F26Dot6  org_dist;
    FT_F26Dot6  control_value_cutin;


    control_value_cutin = exc->GS.control_value_cutin;
    cvtEntry            = (FT_ULong)args[1];
    point               = (FT_UShort)args[0];

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY                        &&
         exc->ignore_x_mode                                 &&
         exc->GS.freeVector.x != 0                          &&
         exc->GS.freeVector.y == 0                          &&
         !( exc->sph_tweak_flags & SPH_TWEAK_NORMAL_ROUND ) )
      control_value_cutin = 0;
#endif  

    if ( BOUNDS( point,     exc->zp0.n_points ) ||
         BOUNDSL( cvtEntry, exc->cvtSize )      )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      goto Fail;
    }

     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     

    distance = exc->func_read_cvt( exc, cvtEntry );

    if ( exc->GS.gep0 == 0 )    
    {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
       
       
      if ( !( SUBPIXEL_HINTING_INFINALITY           &&
              ( exc->ignore_x_mode                &&
                exc->face->sph_compatibility_mode ) ) )
#endif  
        exc->zp0.org[point].x = TT_MulFix14( distance,
                                             exc->GS.freeVector.x );
      exc->zp0.org[point].y = TT_MulFix14( distance,
                                           exc->GS.freeVector.y ),
      exc->zp0.cur[point]   = exc->zp0.org[point];
    }
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY                    &&
         exc->ignore_x_mode                             &&
         ( exc->sph_tweak_flags & SPH_TWEAK_MIAP_HACK ) &&
         distance > 0                                   &&
         exc->GS.freeVector.y != 0                      )
      distance = 0;
#endif  

    org_dist = FAST_PROJECT( &exc->zp0.cur[point] );

    if ( ( exc->opcode & 1 ) != 0 )    
    {
      if ( FT_ABS( distance - org_dist ) > control_value_cutin )
        distance = org_dist;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
      if ( SUBPIXEL_HINTING_INFINALITY &&
           exc->ignore_x_mode          &&
           exc->GS.freeVector.x != 0   )
        distance = Round_None( exc,
                               distance,
                               exc->tt_metrics.compensations[0] );
      else
#endif
        distance = exc->func_round( exc,
                                    distance,
                                    exc->tt_metrics.compensations[0] );
    }

    exc->func_move( exc, &exc->zp0, point, SUB_LONG( distance, org_dist ) );

  Fail:
    exc->GS.rp0 = point;
    exc->GS.rp1 = point;
  }
