  Ins_DELTAP( TT_ExecContext  exc,
              FT_Long*        args )
  {
    FT_ULong   nump, k;
    FT_UShort  A;
    FT_ULong   C, P;
    FT_Long    B;
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    FT_UShort  B1, B2;


    if ( SUBPIXEL_HINTING_INFINALITY                              &&
         exc->ignore_x_mode                                       &&
         exc->iup_called                                          &&
         ( exc->sph_tweak_flags & SPH_TWEAK_NO_DELTAP_AFTER_IUP ) )
      goto Fail;
#endif  

    P    = (FT_ULong)exc->func_cur_ppem( exc );
    nump = (FT_ULong)args[0];    

    for ( k = 1; k <= nump; k++ )
    {
      if ( exc->args < 2 )
      {
        if ( exc->pedantic_hinting )
          exc->error = FT_THROW( Too_Few_Arguments );
        exc->args = 0;
        goto Fail;
      }

      exc->args -= 2;

      A = (FT_UShort)exc->stack[exc->args + 1];
      B = exc->stack[exc->args];

       
       
       
       
       

      if ( !BOUNDS( A, exc->zp0.n_points ) )
      {
        C = ( (FT_ULong)B & 0xF0 ) >> 4;

        switch ( exc->opcode )
        {
        case 0x5D:
          break;

        case 0x71:
          C += 16;
          break;

        case 0x72:
          C += 32;
          break;
        }

        C += exc->GS.delta_base;

        if ( P == C )
        {
          B = ( (FT_ULong)B & 0xF ) - 8;
          if ( B >= 0 )
            B++;
          B *= 1L << ( 6 - exc->GS.delta_shift );

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY

          if ( SUBPIXEL_HINTING_INFINALITY )
          {
             
            if ( !exc->ignore_x_mode                                   ||
                 ( exc->sph_tweak_flags & SPH_TWEAK_ALWAYS_DO_DELTAP ) ||
                 ( exc->is_composite && exc->GS.freeVector.y != 0 )    )
              exc->func_move( exc, &exc->zp0, A, B );

             
             
            else if ( exc->ignore_x_mode && exc->GS.freeVector.y != 0 )
            {
               
              B1 = (FT_UShort)exc->zp0.cur[A].y;

               
               
              if ( !exc->face->sph_compatibility_mode          &&
                   ( exc->zp0.tags[A] & FT_CURVE_TAG_TOUCH_Y ) )
                exc->func_move( exc, &exc->zp0, A, B );

               
              else if ( exc->face->sph_compatibility_mode                        &&
                        !( exc->sph_tweak_flags & SPH_TWEAK_ALWAYS_SKIP_DELTAP ) )
              {
                if ( exc->sph_tweak_flags & SPH_TWEAK_ROUND_NONPIXEL_Y_MOVES )
                  B = FT_PIX_ROUND( B1 + B ) - B1;

                 
                 
                if ( !exc->iup_called                            &&
                     ( exc->zp0.tags[A] & FT_CURVE_TAG_TOUCH_Y ) )
                  exc->func_move( exc, &exc->zp0, A, B );
              }

              B2 = (FT_UShort)exc->zp0.cur[A].y;

               
              if ( exc->GS.freeVector.y != 0                          &&
                   ( ( exc->face->sph_compatibility_mode          &&
                       ( B1 & 63 ) == 0                           &&
                       ( B2 & 63 ) != 0                           ) ||
                     ( ( exc->sph_tweak_flags                   &
                         SPH_TWEAK_SKIP_NONPIXEL_Y_MOVES_DELTAP ) &&
                       ( B1 & 63 ) != 0                           &&
                       ( B2 & 63 ) != 0                           ) ) )
                exc->func_move( exc, &exc->zp0, A, NEG_LONG( B ) );
            }
          }
          else
#endif  

          {

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
             
             
            if ( SUBPIXEL_HINTING_MINIMAL    &&
                 exc->backward_compatibility )
            {
              if ( !( exc->iupx_called && exc->iupy_called )              &&
                   ( ( exc->is_composite && exc->GS.freeVector.y != 0 ) ||
                     ( exc->zp0.tags[A] & FT_CURVE_TAG_TOUCH_Y )        ) )
                exc->func_move( exc, &exc->zp0, A, B );
            }
            else
#endif
              exc->func_move( exc, &exc->zp0, A, B );
          }
        }
      }
      else
        if ( exc->pedantic_hinting )
          exc->error = FT_THROW( Invalid_Reference );
    }

  Fail:
    exc->new_top = exc->args;
  }
