static int rd8x8_c(  void *c, uint8_t *src1, uint8_t *src2, int stride, int h){
    MpegEncContext * const s= (MpegEncContext *)c;
    const uint8_t *scantable= s->intra_scantable.permutated;
    LOCAL_ALIGNED_16(int16_t, temp, [64]);
    LOCAL_ALIGNED_16(uint8_t, lsrc1, [64]);
    LOCAL_ALIGNED_16(uint8_t, lsrc2, [64]);
    int i, last, run, bits, level, distortion, start_i;
    const int esc_length= s->ac_esc_length;
    uint8_t * length;
    uint8_t * last_length;

    av_assert2(h==8);

    copy_block8(lsrc1, src1, 8, stride, 8);
    copy_block8(lsrc2, src2, 8, stride, 8);

    s->dsp.diff_pixels(temp, lsrc1, lsrc2, 8);

    s->block_last_index[0 ]= last= s->fast_dct_quantize(s, temp, 0 , s->qscale, &i);

    bits=0;

    if (s->mb_intra) {
        start_i = 1;
        length     = s->intra_ac_vlc_length;
        last_length= s->intra_ac_vlc_last_length;
        bits+= s->luma_dc_vlc_length[temp[0] + 256];  
    } else {
        start_i = 0;
        length     = s->inter_ac_vlc_length;
        last_length= s->inter_ac_vlc_last_length;
    }

    if(last>=start_i){
        run=0;
        for(i=start_i; i<last; i++){
            int j= scantable[i];
            level= temp[j];

            if(level){
                level+=64;
                if((level&(~127)) == 0){
                    bits+= length[UNI_AC_ENC_INDEX(run, level)];
                }else
                    bits+= esc_length;
                run=0;
            }else
                run++;
        }
        i= scantable[last];

        level= temp[i] + 64;

        av_assert2(level - 64);

        if((level&(~127)) == 0){
            bits+= last_length[UNI_AC_ENC_INDEX(run, level)];
        }else
            bits+= esc_length;

    }

    if(last>=0){
        if(s->mb_intra)
            s->dct_unquantize_intra(s, temp, 0, s->qscale);
        else
            s->dct_unquantize_inter(s, temp, 0, s->qscale);
    }

    s->dsp.idct_add(lsrc2, 8, temp);

    distortion= s->dsp.sse[1](NULL, lsrc2, lsrc1, 8, 8);

    return distortion + ((bits*s->qscale*s->qscale*109 + 64)>>7);
}