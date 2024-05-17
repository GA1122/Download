VOID ixheaacd_shiftrountine(WORD32 *qmf_real, WORD32 *qmf_imag, WORD32 len,
                            WORD32 common_shift) {
  WORD32 treal, timag;
  WORD32 j;

 if (common_shift < 0) {
    WORD32 cshift = -common_shift;
    cshift = ixheaacd_min32(cshift, 31);
 for (j = len - 1; j >= 0; j--) {
      treal = *qmf_real;
      timag = *qmf_imag;

      treal = (ixheaacd_shr32(treal, cshift));
      timag = (ixheaacd_shr32(timag, cshift));

 *qmf_real++ = treal;
 *qmf_imag++ = timag;
 }
 } else {
 for (j = len - 1; j >= 0; j--) {
      treal = (ixheaacd_shl32_sat(*qmf_real, common_shift));
      timag = (ixheaacd_shl32_sat(*qmf_imag, common_shift));
 *qmf_real++ = treal;
 *qmf_imag++ = timag;
 }
 }
}
