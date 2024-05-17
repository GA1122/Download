update_error_state(vpx_codec_alg_priv_t *ctx,
 const struct vpx_internal_error_info *error)
{
 vpx_codec_err_t res;

 if ((res = error->error_code))
        ctx->base.err_detail = error->has_detail
 ? error->detail
 : NULL;

 return res;
}
