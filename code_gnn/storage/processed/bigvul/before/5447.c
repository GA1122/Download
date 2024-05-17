  static void  Ins_SFVFS( INS_ARG )
  {
    Short  S;
    Long   X, Y;

     
    S = (Short)args[1];
    Y = (Long)S;
    S = (Short)args[0];
    X = S;

    if ( NORMalize( X, Y, &CUR.GS.freeVector ) == FAILURE )
      return;

    COMPUTE_Funcs();
  }
