 xps_true_callback_glyph_name(gs_font *pfont, gs_glyph glyph, gs_const_string *pstr)
 {
      
 
     int table_length;
      int table_offset;
  
      ulong format;
    uint numGlyphs;
//     int numGlyphs;
      uint glyph_name_index;
      const byte *postp;  
  
//     if (glyph >= GS_MIN_GLYPH_INDEX) {
//         glyph -= GS_MIN_GLYPH_INDEX;
//     }
// 
       
      if ( pfont->FontType != ft_TrueType )
      {
             pstr->size = strlen((char*)pstr->data);
             return 0;
         }
         else
         {
             return gs_throw1(-1, "glyph index %lu out of range", (ulong)glyph);
         }
     }