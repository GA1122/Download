WORD32 ihevcd_set_default_params(codec_t *ps_codec)
{

    WORD32 ret = IV_SUCCESS;

    ps_codec->e_pic_skip_mode = IVD_SKIP_NONE;
    ps_codec->i4_strd = 0;
    ps_codec->i4_disp_strd = 0;
    ps_codec->i4_header_mode = 0;
    ps_codec->e_pic_out_order = IVD_DISPLAY_FRAME_OUT;
 return ret;
}
