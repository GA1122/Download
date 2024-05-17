  static Short  GetShortIns( EXEC_OP )
  {
     
    CUR.IP += 2;
    return ( CUR.code[CUR.IP-2] << 8) +
             CUR.code[CUR.IP-1];
  }
