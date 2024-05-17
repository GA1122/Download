  static void  Ins_ELSE( INS_ARG )
  {
    Int  nIfs;
    (void)args;

    nIfs = 1;

    do
    {
      if ( SKIP_Code() == FAILURE )
        return;

      switch ( CUR.opcode )
      {
      case 0x58:     
        nIfs++;
        break;

      case 0x59:     
        nIfs--;
        break;
      }
    } while ( nIfs != 0 );
  }
