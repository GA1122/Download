  Ins_SHZ( TT_ExecContext  exc,
           FT_Long*        args )
  {
    TT_GlyphZoneRec  zp;
    FT_UShort        refp;
    FT_F26Dot6       dx,
                     dy;

    FT_UShort        limit, i;


    if ( BOUNDS( args[0], 2 ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    if ( Compute_Point_Displacement( exc, &dx, &dy, &zp, &refp ) )
      return;

     
     
     
     
    if ( exc->GS.gep2 == 0 )
      limit = (FT_UShort)exc->zp2.n_points;
    else if ( exc->GS.gep2 == 1 && exc->zp2.n_contours > 0 )
      limit = (FT_UShort)( exc->zp2.contours[exc->zp2.n_contours - 1] + 1 );
    else
      limit = 0;

     
    for ( i = 0; i < limit; i++ )
    {
      if ( zp.cur != exc->zp2.cur || refp != i )
        Move_Zp2_Point( exc, i, dx, dy, FALSE );
    }
  }
