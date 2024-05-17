  tt_cmap4_get_info( TT_CMap       cmap,
                     TT_CMapInfo  *cmap_info )
  {
    FT_Byte*  p = cmap->data + 4;


    cmap_info->format = 4;
    cmap_info->language = (FT_ULong)TT_PEEK_USHORT( p );

    return SFNT_Err_Ok;
  }