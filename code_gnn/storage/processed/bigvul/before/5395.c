  static void  Ins_IF( INS_ARG )
  {
    Int   nIfs;
    Bool  Out;

    if ( args[0] != 0 )
      return;

    nIfs = 1;
    Out = 0;

    do
    {
      if ( SKIP_Code() == FAILURE )
        return;

      switch ( CUR.opcode )
      {
      case 0x58:       
        nIfs++;
        break;

      case 0x1b:       
        Out = (nIfs == 1);
        break;

      case 0x59:       
        nIfs--;
        Out = (nIfs == 0);
        break;
      }
    } while ( Out == 0 );
  }