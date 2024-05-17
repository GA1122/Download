  cf2_getPpemY( CFF_Decoder*  decoder )
  {
    FT_ASSERT( decoder                          &&
               decoder->builder.face            &&
               decoder->builder.face->root.size );

     
    return cf2_intToFixed(
             decoder->builder.face->root.size->metrics.y_ppem );
  }
