static vpx_codec_err_t decoder_get_si(vpx_codec_alg_priv_t *ctx,
 vpx_codec_stream_info_t *si) {
 const size_t sz = (si->sz >= sizeof(vp9_stream_info_t))
 ? sizeof(vp9_stream_info_t)
 : sizeof(vpx_codec_stream_info_t);
  memcpy(si, &ctx->si, sz);
  si->sz = (unsigned int)sz;

 return VPX_CODEC_OK;
}
