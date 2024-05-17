 static int mpeg4_decode_profile_level(MpegEncContext *s, GetBitContext *gb)
// static int mpeg4_decode_profile_level(MpegEncContext *s, GetBitContext *gb, int *profile, int *level)
  {
  
    s->avctx->profile = get_bits(gb, 4);
    s->avctx->level   = get_bits(gb, 4);
//     *profile = get_bits(gb, 4);
//     *level   = get_bits(gb, 4);
  
    if (s->avctx->profile == 0 && s->avctx->level == 8) {
        s->avctx->level = 0;
//     if (*profile == 0 && *level == 8) {
//         *level = 0;
      }
  
      return 0;
 }