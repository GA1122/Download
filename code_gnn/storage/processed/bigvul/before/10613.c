  Ins_INSTCTRL( TT_ExecContext  exc,
                FT_Long*        args )
  {
    FT_ULong  K, L, Kf;


    K = (FT_ULong)args[1];
    L = (FT_ULong)args[0];

     
     
    if ( K < 1 || K > 3 )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

     
    Kf = 1 << ( K - 1 );

    if ( L != 0 )
    {
       
      if ( L != Kf )
      {
        if ( exc->pedantic_hinting )
          exc->error = FT_THROW( Invalid_Reference );
        return;
      }
    }

    exc->GS.instruct_control &= ~(FT_Byte)Kf;
    exc->GS.instruct_control |= (FT_Byte)L;

    if ( K == 3 )
    {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
       
       
      if ( SUBPIXEL_HINTING_INFINALITY )
        exc->ignore_x_mode = FT_BOOL( L == 4 );
#endif

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
       
       
       
      if ( SUBPIXEL_HINTING_MINIMAL )
        exc->backward_compatibility = !FT_BOOL( L == 4 );
#endif
    }
  }
