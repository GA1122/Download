static int dct_sad8x8_c(  void *c, uint8_t *src1, uint8_t *src2, int stride, int h){
    MpegEncContext * const s= (MpegEncContext *)c;
    LOCAL_ALIGNED_16(int16_t, temp, [64]);

    av_assert2(h==8);

    s->dsp.diff_pixels(temp, src1, src2, stride);
    s->dsp.fdct(temp);
    return s->dsp.sum_abs_dctelem(temp);
}