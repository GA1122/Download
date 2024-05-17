static vpx_codec_err_t decoder_set_fb_fn(
 vpx_codec_alg_priv_t *ctx,
 vpx_get_frame_buffer_cb_fn_t cb_get,
 vpx_release_frame_buffer_cb_fn_t cb_release, void *cb_priv) {
 if (cb_get == NULL || cb_release == NULL) {
 return VPX_CODEC_INVALID_PARAM;
 } else if (ctx->frame_workers == NULL) {
    ctx->get_ext_fb_cb = cb_get;
    ctx->release_ext_fb_cb = cb_release;
    ctx->ext_priv = cb_priv;
 return VPX_CODEC_OK;
 }

 return VPX_CODEC_ERROR;
}
