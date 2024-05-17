  static void skip_FDEF( EXEC_OP )
  {
     

    while ( SKIP_Code() == SUCCESS )
    {
      switch ( CUR.opcode )
      {
      case 0x89:     
      case 0x2c:     
        CUR.error = TT_Err_Nested_DEFS;
        return;

      case 0x2d:    
        return;
      }
    }
  }
