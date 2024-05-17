  T42_GlyphSlot_Done( FT_GlyphSlot  t42slot )        
  {
    T42_GlyphSlot  slot = (T42_GlyphSlot)t42slot;


    FT_Done_GlyphSlot( slot->ttslot );
  }
