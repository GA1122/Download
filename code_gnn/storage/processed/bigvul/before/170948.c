static vpx_codec_err_t vp8_init(vpx_codec_ctx_t *ctx,
 vpx_codec_priv_enc_mr_cfg_t *data)
{
 vpx_codec_err_t res = VPX_CODEC_OK;
 vpx_codec_alg_priv_t *priv = NULL;
 (void) data;

    vp8_rtcd();
    vpx_dsp_rtcd();
    vpx_scale_rtcd();

  
 if (!ctx->priv) {
      vp8_init_ctx(ctx);
      priv = (vpx_codec_alg_priv_t *)ctx->priv;

  
      priv->fragments.count = 0;
  
      priv->fragments.enabled =
 (priv->base.init_flags & VPX_CODEC_USE_INPUT_FRAGMENTS);

  
 } else {
      priv = (vpx_codec_alg_priv_t *)ctx->priv;
 }

    priv->yv12_frame_buffers.use_frame_threads =
 (ctx->priv->init_flags & VPX_CODEC_USE_FRAME_THREADING);

  
    priv->yv12_frame_buffers.use_frame_threads = 0;

 if (priv->yv12_frame_buffers.use_frame_threads &&
 ((ctx->priv->init_flags & VPX_CODEC_USE_ERROR_CONCEALMENT) ||
 (ctx->priv->init_flags & VPX_CODEC_USE_INPUT_FRAGMENTS))) {
  
      res = VPX_CODEC_INVALID_PARAM;
 }

 return res;
}
