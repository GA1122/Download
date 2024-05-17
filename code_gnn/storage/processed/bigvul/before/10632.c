  Ins_MPS( TT_ExecContext  exc,
           FT_Long*        args )
  {
    if ( NO_SUBPIXEL_HINTING )
    {
       
       
      args[0] = exc->func_cur_ppem( exc );
    }
    else
    {
       
       
       
       
      args[0] = exc->pointSize;
    }
  }
