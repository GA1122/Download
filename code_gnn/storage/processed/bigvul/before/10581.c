  Ins_CALL( TT_ExecContext  exc,
            FT_Long*        args )
  {
    FT_ULong       F;
    TT_CallRec*    pCrec;
    TT_DefRecord*  def;


     

    F = (FT_ULong)args[0];
    if ( BOUNDSL( F, exc->maxFunc + 1 ) )
      goto Fail;

     
     
     
     
     
     
     
     

    def = exc->FDefs + F;
    if ( exc->maxFunc + 1 != exc->numFDefs || def->opc != F )
    {
       
      TT_DefRecord*  limit;


      def   = exc->FDefs;
      limit = def + exc->numFDefs;

      while ( def < limit && def->opc != F )
        def++;

      if ( def == limit )
        goto Fail;
    }

     
    if ( !def->active )
      goto Fail;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY                                    &&
         exc->ignore_x_mode                                             &&
         ( ( exc->iup_called                                        &&
             ( exc->sph_tweak_flags & SPH_TWEAK_NO_CALL_AFTER_IUP ) ) ||
           ( def->sph_fdef_flags & SPH_FDEF_VACUFORM_ROUND_1 )        ) )
      goto Fail;
    else
      exc->sph_in_func_flags = def->sph_fdef_flags;
#endif  

     
    if ( exc->callTop >= exc->callSize )
    {
      exc->error = FT_THROW( Stack_Overflow );
      return;
    }

    pCrec = exc->callStack + exc->callTop;

    pCrec->Caller_Range = exc->curRange;
    pCrec->Caller_IP    = exc->IP + 1;
    pCrec->Cur_Count    = 1;
    pCrec->Def          = def;

    exc->callTop++;

    Ins_Goto_CodeRange( exc, def->range, def->start );

    exc->step_ins = FALSE;

    return;

  Fail:
    exc->error = FT_THROW( Invalid_Reference );
  }
