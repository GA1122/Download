static int dct264_sad8x8_c(  void *c, uint8_t *src1, uint8_t *src2, int stride, int h){
    MpegEncContext * const s= (MpegEncContext *)c;
    int16_t dct[8][8];
    int i;
    int sum=0;

    s->dsp.diff_pixels(dct[0], src1, src2, stride);

#define SRC(x) dct[i][x]
#define DST(x,v) dct[i][x]= v
    for( i = 0; i < 8; i++ )
        DCT8_1D
#undef SRC
#undef DST

#define SRC(x) dct[x][i]
#define DST(x,v) sum += FFABS(v)
    for( i = 0; i < 8; i++ )
        DCT8_1D
#undef SRC
#undef DST
    return sum;
}