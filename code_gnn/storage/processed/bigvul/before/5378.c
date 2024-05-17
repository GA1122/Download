  static void  Ins_ENDF( INS_ARG )
  {
    PCallRecord  pRec;
     (void)args;

    if ( CUR.callTop <= 0 )      
    {
      CUR.error = TT_Err_ENDF_In_Exec_Stream;
      return;
    }

    CUR.callTop--;

    pRec = &CUR.callStack[CUR.callTop];

    pRec->Cur_Count--;

    CUR.step_ins = FALSE;

    if ( pRec->Cur_Count > 0 )
    {
      CUR.callTop++;
      CUR.IP = pRec->Cur_Restart;
    }
    else
       
      INS_Goto_CodeRange( pRec->Caller_Range,
                          pRec->Caller_IP );

     

     
     
     
     
     
  }
