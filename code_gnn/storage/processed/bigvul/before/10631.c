  Ins_MPPEM( TT_ExecContext  exc,
             FT_Long*        args )
  {
    args[0] = exc->func_cur_ppem( exc );
  }
