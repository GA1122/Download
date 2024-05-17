  Ins_GETINFO( INS_ARG )
  {
    FT_Long  K;


    K = 0;

     
    if ( ( args[0] & 1 ) != 0 )
      K = 35;

     
    if ( ( args[0] & 2 ) != 0 && CUR.tt_metrics.rotated )
      K |= 0x80;

     
    if ( ( args[0] & 4 ) != 0 && CUR.tt_metrics.stretched )
      K |= 1 << 8;

     
    if ( ( args[0] & 32 ) != 0 && CUR.grayscale )
      K |= 1 << 12;

    args[0] = K;
  }
