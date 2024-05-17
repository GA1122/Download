  Ins_ENDF( TT_ExecContext  exc )
  {
    TT_CallRec*  pRec;


#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    exc->sph_in_func_flags = 0x0000;
#endif  

    if ( exc->callTop <= 0 )      
    {
      exc->error = FT_THROW( ENDF_In_Exec_Stream );
      return;
    }

    exc->callTop--;

    pRec = &exc->callStack[exc->callTop];

    pRec->Cur_Count--;

    exc->step_ins = FALSE;

    if ( pRec->Cur_Count > 0 )
    {
      exc->callTop++;
      exc->IP = pRec->Def->start;
    }
    else
       
      Ins_Goto_CodeRange( exc, pRec->Caller_Range, pRec->Caller_IP );

     

     
     
     
     
     
  }
