  ft_glyphslot_free_bitmap( FT_GlyphSlot  slot )
  {
    if ( slot->internal && ( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Memory  memory = FT_FACE_MEMORY( slot->face );


      FT_FREE( slot->bitmap.buffer );
      slot->internal->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    else
    {
       
       
      slot->bitmap.buffer = NULL;
    }
  }
