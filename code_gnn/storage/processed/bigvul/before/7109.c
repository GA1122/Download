  tt_cmap14_get_info( TT_CMap       cmap,
                      TT_CMapInfo  *cmap_info )
  {
    FT_UNUSED( cmap );

    cmap_info->format   = 14;
     
    cmap_info->language = 0xFFFFFFFFUL;

    return FT_Err_Ok;
  }