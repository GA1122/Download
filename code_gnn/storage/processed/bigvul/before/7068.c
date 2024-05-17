  t1_done_loader( T1_Loader  loader )
  {
    T1_Parser  parser = &loader->parser;


     
    T1_Release_Table( &loader->encoding_table );
    T1_Release_Table( &loader->charstrings );
    T1_Release_Table( &loader->glyph_names );
    T1_Release_Table( &loader->swap_table );
    T1_Release_Table( &loader->subrs );

     
    T1_Finalize_Parser( parser );
  }
