static vpx_codec_err_t vp8_get_si(vpx_codec_alg_priv_t *ctx,
 vpx_codec_stream_info_t *si)
{

 unsigned int sz;

 if (si->sz >= sizeof(vp8_stream_info_t))
        sz = sizeof(vp8_stream_info_t);
 else
        sz = sizeof(vpx_codec_stream_info_t);

    memcpy(si, &ctx->si, sz);
    si->sz = sz;

 return VPX_CODEC_OK;
}
