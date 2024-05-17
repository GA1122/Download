  tt_cmap4_char_index( TT_CMap    cmap,
                       FT_UInt32  char_code )
  {
    if ( char_code >= 0x10000UL )
      return 0;

    if ( cmap->flags & TT_CMAP_FLAG_UNSORTED )
      return tt_cmap4_char_map_linear( cmap, &char_code, 0 );
    else
      return tt_cmap4_char_map_binary( cmap, &char_code, 0 );
  }
