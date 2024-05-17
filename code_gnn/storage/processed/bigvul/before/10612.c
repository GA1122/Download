  Ins_IF( TT_ExecContext  exc,
          FT_Long*        args )
  {
    FT_Int   nIfs;
    FT_Bool  Out;


    if ( args[0] != 0 )
      return;

    nIfs = 1;
    Out = 0;

    do
    {
      if ( SkipCode( exc ) == FAILURE )
        return;

      switch ( exc->opcode )
      {
      case 0x58:       
        nIfs++;
        break;

      case 0x1B:       
        Out = FT_BOOL( nIfs == 1 );
        break;

      case 0x59:       
        nIfs--;
        Out = FT_BOOL( nIfs == 0 );
        break;
      }
    } while ( Out == 0 );
  }
