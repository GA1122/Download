  cf2_getScaleAndHintFlag( CFF_Decoder*  decoder,
                           CF2_Fixed*    x_scale,
                           CF2_Fixed*    y_scale,
                           FT_Bool*      hinted,
                           FT_Bool*      scaled )
  {
    FT_ASSERT( decoder && decoder->builder.glyph );

     
    *hinted = decoder->builder.glyph->hint;
    *scaled = decoder->builder.glyph->scaled;

    if ( *hinted )
    {
      *x_scale = FT_DivFix( decoder->builder.glyph->x_scale,
                            cf2_intToFixed( 64 ) );
      *y_scale = FT_DivFix( decoder->builder.glyph->y_scale,
                            cf2_intToFixed( 64 ) );
    }
    else
    {
       
       

      *x_scale = 0x0400;    
      *y_scale = 0x0400;
    }
  }
