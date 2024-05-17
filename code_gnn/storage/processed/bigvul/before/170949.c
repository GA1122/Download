static void vp8_init_ctx(vpx_codec_ctx_t *ctx)
{
 vpx_codec_alg_priv_t *priv =
 (vpx_codec_alg_priv_t *)vpx_calloc(1, sizeof(*priv));

    ctx->priv = (vpx_codec_priv_t *)priv;
    ctx->priv->init_flags = ctx->init_flags;

    priv->si.sz = sizeof(priv->si);
    priv->decrypt_cb = NULL;
    priv->decrypt_state = NULL;

 if (ctx->config.dec)
 {
  
        priv->cfg = *ctx->config.dec;
        ctx->config.dec = &priv->cfg;
 }
}
