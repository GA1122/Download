  static Bool  Ins_Goto_CodeRange( EXEC_OPS Int  aRange, Int  aIP )
  {
    TCodeRange*  WITH;

    if ( aRange < 1 || aRange > 3 )
    {
      CUR.error = TT_Err_Bad_Argument;
      return FAILURE;
    }

    WITH = &CUR.codeRangeTable[aRange - 1];

    if ( WITH->Base == NULL )      
    {
      CUR.error = TT_Err_Invalid_CodeRange;
      return FAILURE;
    }

     
     
     

    if ( aIP > WITH->Size )
    {
      CUR.error = TT_Err_Code_Overflow;
      return FAILURE;
    }

    CUR.code     = WITH->Base;
    CUR.codeSize = WITH->Size;
    CUR.IP       = aIP;
    CUR.curRange = aRange;

    return SUCCESS;
  }
