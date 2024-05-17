static vpx_codec_err_t decoder_init(vpx_codec_ctx_t *ctx,
 vpx_codec_priv_enc_mr_cfg_t *data) {
 (void)data;

 if (!ctx->priv) {
 vpx_codec_alg_priv_t *const priv =
 (vpx_codec_alg_priv_t *)vpx_calloc(1, sizeof(*priv));
 if (priv == NULL)
 return VPX_CODEC_MEM_ERROR;

    ctx->priv = (vpx_codec_priv_t *)priv;
    ctx->priv->init_flags = ctx->init_flags;
    priv->si.sz = sizeof(priv->si);
    priv->flushed = 0;
    priv->frame_parallel_decode =
 (ctx->config.dec && (ctx->config.dec->threads > 1) &&
 (ctx->init_flags & VPX_CODEC_USE_FRAME_THREADING)) ? 1 : 0;
 if (ctx->config.dec) {
      priv->cfg = *ctx->config.dec;
      ctx->config.dec = &priv->cfg;
 }
 }

 return VPX_CODEC_OK;
}
