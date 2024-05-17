  Ins_IDEF( INS_ARG )
  {
    TT_DefRecord*  def;
    TT_DefRecord*  limit;


     

    def   = CUR.IDefs;
    limit = def + CUR.numIDefs;

    for ( ; def < limit; def++ )
      if ( def->opc == (FT_ULong)args[0] )
        break;

    if ( def == limit )
    {
       
      if ( CUR.numIDefs >= CUR.maxIDefs )
      {
        CUR.error = TT_Err_Too_Many_Instruction_Defs;
        return;
      }
      CUR.numIDefs++;
    }

     
    if ( 0 > args[0] || args[0] > 0x00FF )
    {
      CUR.error = TT_Err_Too_Many_Instruction_Defs;
      return;
    }

    def->opc    = (FT_Byte)args[0];
    def->start  = CUR.IP + 1;
    def->range  = CUR.curRange;
    def->active = TRUE;

    if ( (FT_ULong)args[0] > CUR.maxIns )
      CUR.maxIns = (FT_Byte)args[0];

     
     

    while ( SKIP_Code() == SUCCESS )
    {
      switch ( CUR.opcode )
      {
      case 0x89:    
      case 0x2C:    
        CUR.error = TT_Err_Nested_DEFS;
        return;
      case 0x2D:    
        return;
      }
    }
  }
