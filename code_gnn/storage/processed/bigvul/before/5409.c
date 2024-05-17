  static void  Ins_MIAP( INS_ARG )
  {
    Int         cvtEntry, point;
    TT_F26Dot6  distance,
                org_dist;

    cvtEntry = (Int)args[1];
    point    = (Int)args[0];

    if ( BOUNDS( args[0], CUR.zp0.n_points ) ||
         BOUNDS( args[1], CUR.cvtSize )      )
    {
         
      return;
    }

     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     

    distance = CUR_Func_read_cvt( cvtEntry );

    DBG_PRINT3(" cvtEntry=%d point=%d distance=%d", cvtEntry, point, distance);

    if ( CUR.GS.gep0 == 0 )    
    {
      CUR.zp0.org_x[point] = MulDiv_Round( CUR.GS.freeVector.x,
                                           distance, 0x4000L );
      CUR.zp0.cur_x[point] = CUR.zp0.org_x[point];

      CUR.zp0.org_y[point] = MulDiv_Round( CUR.GS.freeVector.y,
                                           distance, 0x4000L );
      CUR.zp0.cur_y[point] = CUR.zp0.org_y[point];
    }

    org_dist = CUR_Func_project( CUR.zp0.cur_x[point],
                                 CUR.zp0.cur_y[point] );

    if ( (CUR.opcode & 1) != 0 )    
    {
      if ( ABS(distance - org_dist) > CUR.GS.control_value_cutin )
        distance = org_dist;

      distance = CUR_Func_round( distance, CUR.metrics.compensations[0] );
    }

    CUR_Func_move( &CUR.zp0, point, distance - org_dist );

    CUR.GS.rp0 = point;
    CUR.GS.rp1 = point;
  }
