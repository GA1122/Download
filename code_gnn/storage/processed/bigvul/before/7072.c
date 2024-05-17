  t42_loader_done( T42_Loader  loader )
  {
    T42_Parser  parser = &loader->parser;


     
    T1_Release_Table( &loader->encoding_table );
    T1_Release_Table( &loader->charstrings );
    T1_Release_Table( &loader->glyph_names );
    T1_Release_Table( &loader->swap_table );

     
    t42_parser_done( parser );
  }
