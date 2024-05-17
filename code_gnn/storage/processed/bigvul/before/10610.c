  Ins_Goto_CodeRange( TT_ExecContext  exc,
                      FT_Int          aRange,
                      FT_Long         aIP )
  {
    TT_CodeRange*  range;


    if ( aRange < 1 || aRange > 3 )
    {
      exc->error = FT_THROW( Bad_Argument );
      return FAILURE;
    }

    range = &exc->codeRangeTable[aRange - 1];

    if ( !range->base )      
    {
      exc->error = FT_THROW( Invalid_CodeRange );
      return FAILURE;
    }

     
     
     

    if ( aIP > range->size )
    {
      exc->error = FT_THROW( Code_Overflow );
      return FAILURE;
    }

    exc->code     = range->base;
    exc->codeSize = range->size;
    exc->IP       = aIP;
    exc->curRange = aRange;

    return SUCCESS;
  }
