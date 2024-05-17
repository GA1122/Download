static int dct_max8x8_c(  void *c, uint8_t *src1, uint8_t *src2, int stride, int h){
    MpegEncContext * const s= (MpegEncContext *)c;
    LOCAL_ALIGNED_16(int16_t, temp, [64]);
    int sum=0, i;

    av_assert2(h==8);

    s->dsp.diff_pixels(temp, src1, src2, stride);
    s->dsp.fdct(temp);

    for(i=0; i<64; i++)
        sum= FFMAX(sum, FFABS(temp[i]));

    return sum;
}