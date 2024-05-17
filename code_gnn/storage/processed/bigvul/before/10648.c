  Ins_ROFF( TT_ExecContext  exc )
  {
    exc->GS.round_state = TT_Round_Off;
    exc->func_round     = (TT_Round_Func)Round_None;
  }
