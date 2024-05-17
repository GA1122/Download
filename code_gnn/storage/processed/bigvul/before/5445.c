  static void  Ins_SDPVTL( INS_ARG )
  {
    Long  A, B, C;
    Long  p1, p2;    

    p1 = args[1];
    p2 = args[0];

    if ( BOUNDS( p2, CUR.zp1.n_points ) ||
         BOUNDS( p1, CUR.zp2.n_points ) )
    {
      CUR.error = TT_Err_Invalid_Reference;
      return;
    }

    A = CUR.zp1.org_x[p2] - CUR.zp2.org_x[p1];
    B = CUR.zp1.org_y[p2] - CUR.zp2.org_y[p1];

    if ( (CUR.opcode & 1) != 0 )
    {
      C =  B;    
      B =  A;
      A = -C;
    }

    if ( NORMalize( A, B, &CUR.GS.dualVector ) == FAILURE )
      return;

    A = CUR.zp1.cur_x[p2] - CUR.zp2.cur_x[p1];
    B = CUR.zp1.cur_y[p2] - CUR.zp2.cur_y[p1];

    if ( (CUR.opcode & 1) != 0 )
    {
      C =  B;    
      B =  A;
      A = -C;
    }

    if ( NORMalize( A, B, &CUR.GS.projVector ) == FAILURE )
      return;

    COMPUTE_Funcs();
  }
