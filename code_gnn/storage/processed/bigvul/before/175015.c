WORD16  impeg2d_get_luma_dc_diff(stream_t *ps_stream)
{
    UWORD16 u2_dc_size;
    WORD16  i2_dc_diff;

    u2_dc_size = impeg2d_dec_vld_symbol(ps_stream,gai2_impeg2d_dct_dc_size[0],
                        MPEG2_DCT_DC_LUMA_SIZE_LEN) +
                        MPEG2_DCT_DC_SIZE_OFFSET;
 if (u2_dc_size != 0)
 {
        i2_dc_diff = impeg2d_bit_stream_get(ps_stream,u2_dc_size);
 if ((i2_dc_diff & (1 << (u2_dc_size - 1))) == 0)
            i2_dc_diff -= (1 << u2_dc_size) - 1;
 }
 else
 {
        i2_dc_diff = 0;
 }
 return i2_dc_diff;
}
