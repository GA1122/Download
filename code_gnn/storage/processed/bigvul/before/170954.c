static vpx_codec_err_t vp8_set_dbg_color_ref_frame(vpx_codec_alg_priv_t *ctx,
                                                   va_list args) {
#if CONFIG_POSTPROC_VISUALIZER && CONFIG_POSTPROC
  ctx->dbg_color_ref_frame_flag = va_arg(args, int);
 return VPX_CODEC_OK;
#else
 (void)ctx;
 (void)args;
 return VPX_CODEC_INCAPABLE;
#endif
}
