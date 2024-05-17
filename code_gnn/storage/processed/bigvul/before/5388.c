  static void  Ins_GETINFO( INS_ARG )
  {
    Long  K;

    K = 0;

     
     
    if ( (args[0] & 1) != 0 )
      K = 3;

     
    if ( CUR.metrics.rotated )
      K |= 0x80;

     
    if ( CUR.metrics.stretched )
      K |= 0x100;

    args[0] = K;
  }
