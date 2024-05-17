  bdf_free_font( bdf_font_t*  font )
  {
    bdf_property_t*  prop;
    unsigned long    i;
    bdf_glyph_t*     glyphs;
    FT_Memory        memory;


    if ( font == 0 )
      return;

    memory = font->memory;

    FT_FREE( font->name );

     
    if ( font->internal )
    {
      hash_free( (hashtable *)font->internal, memory );
      FT_FREE( font->internal );
    }

     
    FT_FREE( font->comments );

     
    for ( i = 0; i < font->props_size; i++ )
    {
      if ( font->props[i].format == BDF_ATOM )
        FT_FREE( font->props[i].value.atom );
    }

    FT_FREE( font->props );

     
    for ( i = 0, glyphs = font->glyphs;
          i < font->glyphs_used; i++, glyphs++ )
    {
      FT_FREE( glyphs->name );
      FT_FREE( glyphs->bitmap );
    }

    for ( i = 0, glyphs = font->unencoded; i < font->unencoded_used;
          i++, glyphs++ )
    {
      FT_FREE( glyphs->name );
      FT_FREE( glyphs->bitmap );
    }

    FT_FREE( font->glyphs );
    FT_FREE( font->unencoded );

     
    for ( i = 0, glyphs = font->overflow.glyphs;
          i < font->overflow.glyphs_used; i++, glyphs++ )
    {
      FT_FREE( glyphs->name );
      FT_FREE( glyphs->bitmap );
    }

    FT_FREE( font->overflow.glyphs );

     
    hash_free( &(font->proptbl), memory );

     
    for ( prop = font->user_props, i = 0;
          i < font->nuser_props; i++, prop++ )
    {
      FT_FREE( prop->name );
      if ( prop->format == BDF_ATOM )
        FT_FREE( prop->value.atom );
    }

    FT_FREE( font->user_props );

       
  }
