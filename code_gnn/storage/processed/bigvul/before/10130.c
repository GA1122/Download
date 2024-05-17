  Ins_MIAP( INS_ARG )
  {
    FT_ULong    cvtEntry;
    FT_UShort   point;
    FT_F26Dot6  distance,
                org_dist;


    cvtEntry = (FT_ULong)args[1];
    point    = (FT_UShort)args[0];

    if ( BOUNDS( point,    CUR.zp0.n_points ) ||
         BOUNDS( cvtEntry, CUR.cvtSize )      )
    {
      if ( CUR.pedantic_hinting )
        CUR.error = TT_Err_Invalid_Reference;
      return;
    }

     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     

    distance = CUR_Func_read_cvt( cvtEntry );

    if ( CUR.GS.gep0 == 0 )    
    {
      CUR.zp0.org[point].x = TT_MulFix14( (FT_UInt32)distance, CUR.GS.freeVector.x );
      CUR.zp0.org[point].y = TT_MulFix14( (FT_UInt32)distance, CUR.GS.freeVector.y ),
      CUR.zp0.cur[point]   = CUR.zp0.org[point];
    }

    org_dist = CUR_fast_project( &CUR.zp0.cur[point] );

    if ( ( CUR.opcode & 1 ) != 0 )    
    {
      if ( FT_ABS( distance - org_dist ) > CUR.GS.control_value_cutin )
        distance = org_dist;

      distance = CUR_Func_round( distance, CUR.tt_metrics.compensations[0] );
    }

    CUR_Func_move( &CUR.zp0, point, distance - org_dist );

    CUR.GS.rp0 = point;
    CUR.GS.rp1 = point;
  }
