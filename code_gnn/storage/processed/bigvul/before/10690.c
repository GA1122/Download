  Ins_UNKNOWN( TT_ExecContext  exc )
  {
    TT_DefRecord*  def   = exc->IDefs;
    TT_DefRecord*  limit = def + exc->numIDefs;


    for ( ; def < limit; def++ )
    {
      if ( (FT_Byte)def->opc == exc->opcode && def->active )
      {
        TT_CallRec*  call;


        if ( exc->callTop >= exc->callSize )
        {
          exc->error = FT_THROW( Stack_Overflow );
          return;
        }

        call = exc->callStack + exc->callTop++;

        call->Caller_Range = exc->curRange;
        call->Caller_IP    = exc->IP + 1;
        call->Cur_Count    = 1;
        call->Def          = def;

        Ins_Goto_CodeRange( exc, def->range, def->start );

        exc->step_ins = FALSE;
        return;
      }
    }

    exc->error = FT_THROW( Invalid_Opcode );
  }
