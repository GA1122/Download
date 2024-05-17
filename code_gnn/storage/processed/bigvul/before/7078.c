  t42_parser_done( T42_Parser  parser )
  {
    FT_Memory  memory = parser->root.memory;


     
    if ( !parser->in_memory )
      FT_FREE( parser->base_dict );

    parser->root.funcs.done( &parser->root );
  }
