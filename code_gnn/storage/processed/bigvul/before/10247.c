  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
     

    return FT_Lookup_Renderer( library, format, 0 );
  }
