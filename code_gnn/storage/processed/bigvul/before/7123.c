  T42_GlyphSlot_Init( FT_GlyphSlot  t42slot )         
  {
    T42_GlyphSlot  slot    = (T42_GlyphSlot)t42slot;
    FT_Face        face    = t42slot->face;
    T42_Face       t42face = (T42_Face)face;
    FT_GlyphSlot   ttslot;
    FT_Error       error   = FT_Err_Ok;


    if ( face->glyph == NULL )
    {
       
      slot->ttslot = t42face->ttf_face->glyph;
    }
    else
    {
      error = FT_New_GlyphSlot( t42face->ttf_face, &ttslot );
      slot->ttslot = ttslot;
    }

    return error;
  }
