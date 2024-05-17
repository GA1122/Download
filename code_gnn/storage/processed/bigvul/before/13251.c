  cff_parser_init( CFF_Parser  parser,
                   FT_UInt     code,
                   void*       object,
                   FT_Library  library,
                   FT_UInt     stackSize,
                   FT_UShort   num_designs,
                   FT_UShort   num_axes )
  {
    FT_Memory  memory = library->memory;     
    FT_Error   error;                        


    FT_ZERO( parser );

#if 0
    parser->top         = parser->stack;
#endif
    parser->object_code = code;
    parser->object      = object;
    parser->library     = library;
    parser->num_designs = num_designs;
    parser->num_axes    = num_axes;

     
    if ( FT_NEW_ARRAY( parser->stack, stackSize ) )
    {
      FT_FREE( parser->stack );
      goto Exit;
    }

    parser->stackSize = stackSize;
    parser->top       = parser->stack;     

  Exit:
    return error;
  }
