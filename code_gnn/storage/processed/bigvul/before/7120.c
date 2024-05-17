  T42_Face_Done( FT_Face  t42face )
  {
    T42_Face     face = (T42_Face)t42face;
    T1_Font      type1;
    PS_FontInfo  info;
    FT_Memory    memory;


    if ( !face )
      return;

    type1  = &face->type1;
    info   = &type1->font_info;
    memory = face->root.memory;

     
    if ( face->ttf_face )
      FT_Done_Face( face->ttf_face );

     
    FT_FREE( info->version );
    FT_FREE( info->notice );
    FT_FREE( info->full_name );
    FT_FREE( info->family_name );
    FT_FREE( info->weight );

     
    FT_FREE( type1->charstrings_len );
    FT_FREE( type1->charstrings );
    FT_FREE( type1->glyph_names );

    FT_FREE( type1->charstrings_block );
    FT_FREE( type1->glyph_names_block );

    FT_FREE( type1->encoding.char_index );
    FT_FREE( type1->encoding.char_name );
    FT_FREE( type1->font_name );

    FT_FREE( face->ttf_data );

#if 0
     
    if ( face->afm_data )
      T1_Done_AFM( memory, (T1_AFM*)face->afm_data );
#endif

     
    FT_FREE( face->unicode_map.maps );
    face->unicode_map.num_maps = 0;

    face->root.family_name = 0;
    face->root.style_name  = 0;
  }
