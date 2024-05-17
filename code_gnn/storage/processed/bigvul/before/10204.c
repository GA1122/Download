  Read_CVT_Stretched( EXEC_OP_ FT_ULong  idx )
  {
    return TT_MULFIX( CUR.cvt[idx], CURRENT_Ratio() );
  }
