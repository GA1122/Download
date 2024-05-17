  T1_Finalize_Parser( T1_Parser  parser )
  {
    FT_Memory  memory = parser->root.memory;


     
    FT_FREE( parser->private_dict );

     
    if ( !parser->in_memory )
      FT_FREE( parser->base_dict );

    parser->root.funcs.done( &parser->root );
  }
