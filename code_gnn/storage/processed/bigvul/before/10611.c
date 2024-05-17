  Ins_IDEF( TT_ExecContext  exc,
            FT_Long*        args )
  {
    TT_DefRecord*  def;
    TT_DefRecord*  limit;


     
    if ( exc->curRange == tt_coderange_glyph )
    {
      exc->error = FT_THROW( DEF_In_Glyf_Bytecode );
      return;
    }

     

    def   = exc->IDefs;
    limit = def + exc->numIDefs;

    for ( ; def < limit; def++ )
      if ( def->opc == (FT_ULong)args[0] )
        break;

    if ( def == limit )
    {
       
      if ( exc->numIDefs >= exc->maxIDefs )
      {
        exc->error = FT_THROW( Too_Many_Instruction_Defs );
        return;
      }
      exc->numIDefs++;
    }

     
    if ( 0 > args[0] || args[0] > 0x00FF )
    {
      exc->error = FT_THROW( Too_Many_Instruction_Defs );
      return;
    }

    def->opc    = (FT_Byte)args[0];
    def->start  = exc->IP + 1;
    def->range  = exc->curRange;
    def->active = TRUE;

    if ( (FT_ULong)args[0] > exc->maxIns )
      exc->maxIns = (FT_Byte)args[0];

     
     

    while ( SkipCode( exc ) == SUCCESS )
    {
      switch ( exc->opcode )
      {
      case 0x89:    
      case 0x2C:    
        exc->error = FT_THROW( Nested_DEFS );
        return;
      case 0x2D:    
        def->end = exc->IP;
        return;
      }
    }
  }
