  Ins_SHPIX( TT_ExecContext  exc,
             FT_Long*        args )
  {
    FT_F26Dot6  dx, dy;
    FT_UShort   point;
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    FT_Int      B1, B2;
#endif
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
    FT_Bool     in_twilight = FT_BOOL( exc->GS.gep0 == 0 ||
                                       exc->GS.gep1 == 0 ||
                                       exc->GS.gep2 == 0 );
#endif



    if ( exc->top < exc->GS.loop + 1 )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      goto Fail;
    }

    dx = TT_MulFix14( args[0], exc->GS.freeVector.x );
    dy = TT_MulFix14( args[0], exc->GS.freeVector.y );

    while ( exc->GS.loop > 0 )
    {
      exc->args--;

      point = (FT_UShort)exc->stack[exc->args];

      if ( BOUNDS( point, exc->zp2.n_points ) )
      {
        if ( exc->pedantic_hinting )
        {
          exc->error = FT_THROW( Invalid_Reference );
          return;
        }
      }
      else
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
      if ( SUBPIXEL_HINTING_INFINALITY )
      {
         
         
         
         
         
         
         

        if ( exc->ignore_x_mode )
        {
           
          if ( exc->GS.freeVector.y != 0 )
            B1 = exc->zp2.cur[point].y;
          else
            B1 = exc->zp2.cur[point].x;

          if ( !exc->face->sph_compatibility_mode &&
               exc->GS.freeVector.y != 0          )
          {
            Move_Zp2_Point( exc, point, dx, dy, TRUE );

             
            if ( exc->GS.freeVector.y != 0 )
            {
              B2 = exc->zp2.cur[point].y;

               
              if ( ( exc->sph_tweak_flags & SPH_TWEAK_SKIP_NONPIXEL_Y_MOVES ) &&
                   ( B1 & 63 ) != 0                                           &&
                   ( B2 & 63 ) != 0                                           &&
                   B1 != B2                                                   )
                Move_Zp2_Point( exc,
                                point,
                                NEG_LONG( dx ),
                                NEG_LONG( dy ),
                                TRUE );
            }
          }
          else if ( exc->face->sph_compatibility_mode )
          {
            if ( exc->sph_tweak_flags & SPH_TWEAK_ROUND_NONPIXEL_Y_MOVES )
            {
              dx = FT_PIX_ROUND( B1 + dx ) - B1;
              dy = FT_PIX_ROUND( B1 + dy ) - B1;
            }

             
            if ( exc->iup_called                                          &&
                 ( ( exc->sph_in_func_flags & SPH_FDEF_INLINE_DELTA_1 ) ||
                   ( exc->sph_in_func_flags & SPH_FDEF_INLINE_DELTA_2 ) ) )
              goto Skip;

            if ( !( exc->sph_tweak_flags & SPH_TWEAK_ALWAYS_SKIP_DELTAP ) &&
                  ( ( exc->is_composite && exc->GS.freeVector.y != 0 ) ||
                    ( exc->zp2.tags[point] & FT_CURVE_TAG_TOUCH_Y )    ||
                    ( exc->sph_tweak_flags & SPH_TWEAK_DO_SHPIX )      )  )
              Move_Zp2_Point( exc, point, 0, dy, TRUE );

             
            if ( exc->GS.freeVector.y != 0 )
            {
              B2 = exc->zp2.cur[point].y;

               
              if ( ( B1 & 63 ) == 0 &&
                   ( B2 & 63 ) != 0 &&
                   B1 != B2         )
                Move_Zp2_Point( exc, point, 0, NEG_LONG( dy ), TRUE );
            }
          }
          else if ( exc->sph_in_func_flags & SPH_FDEF_TYPEMAN_DIAGENDCTRL )
            Move_Zp2_Point( exc, point, dx, dy, TRUE );
        }
        else
          Move_Zp2_Point( exc, point, dx, dy, TRUE );
      }
      else
#endif
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
      if ( SUBPIXEL_HINTING_MINIMAL    &&
           exc->backward_compatibility )
      {
         
         
         
         
         
        if ( in_twilight                                                ||
             ( !( exc->iupx_called && exc->iupy_called )              &&
               ( ( exc->is_composite && exc->GS.freeVector.y != 0 ) ||
                 ( exc->zp2.tags[point] & FT_CURVE_TAG_TOUCH_Y )    ) ) )
          Move_Zp2_Point( exc, point, 0, dy, TRUE );
      }
      else
#endif
        Move_Zp2_Point( exc, point, dx, dy, TRUE );

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    Skip:
#endif
      exc->GS.loop--;
    }

  Fail:
    exc->GS.loop = 1;
    exc->new_top = exc->args;
  }
