  t1_init_loader( T1_Loader  loader,
                  T1_Face    face )
  {
    FT_UNUSED( face );

    FT_MEM_ZERO( loader, sizeof ( *loader ) );
    loader->num_glyphs = 0;
    loader->num_chars  = 0;

     
    loader->encoding_table.init  = 0;
    loader->charstrings.init     = 0;
    loader->glyph_names.init     = 0;
    loader->subrs.init           = 0;
    loader->swap_table.init      = 0;
    loader->fontdata             = 0;
    loader->keywords_encountered = 0;
  }
