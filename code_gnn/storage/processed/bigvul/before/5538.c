  static void  Ins_JMPR( INS_ARG )
  {
    CUR.IP      += (Int)(args[0]);
    CUR.step_ins = FALSE;

    if(CUR.IP > CUR.codeSize ||
       (CUR.code[CUR.IP] != 0x2D && CUR.code[CUR.IP - 1] == 0x2D))
     
      CUR.IP -= 1;
  }
