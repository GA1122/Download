  cf2_getBlueMetrics( CFF_Decoder*  decoder,
                      CF2_Fixed*    blueScale,
                      CF2_Fixed*    blueShift,
                      CF2_Fixed*    blueFuzz )
  {
    FT_ASSERT( decoder && decoder->current_subfont );

    *blueScale = FT_DivFix(
                   decoder->current_subfont->private_dict.blue_scale,
                   cf2_intToFixed( 1000 ) );
    *blueShift = cf2_intToFixed(
                   decoder->current_subfont->private_dict.blue_shift );
    *blueFuzz  = cf2_intToFixed(
                   decoder->current_subfont->private_dict.blue_fuzz );
  }
