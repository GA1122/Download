  Ins_NPUSHB( TT_ExecContext  exc,
              FT_Long*        args )
  {
    FT_UShort  L, K;


    L = (FT_UShort)exc->code[exc->IP + 1];

    if ( BOUNDS( L, exc->stackSize + 1 - exc->top ) )
    {
      exc->error = FT_THROW( Stack_Overflow );
      return;
    }

    for ( K = 1; K <= L; K++ )
      args[K - 1] = exc->code[exc->IP + K + 1];

    exc->new_top += L;
  }
