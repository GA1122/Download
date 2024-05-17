  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    if ( !charmap || !charmap->face )
      return -1;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    if ( service == NULL )
      return -1;
    if ( service->get_cmap_info( charmap, &cmap_info ))
      return -1;

    return cmap_info.format;
  }
