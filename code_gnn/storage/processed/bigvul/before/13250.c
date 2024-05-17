  cff_parser_done( CFF_Parser  parser )
  {
    FT_Memory  memory = parser->library->memory;     


    FT_FREE( parser->stack );
  }
