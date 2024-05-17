update_fragments(vpx_codec_alg_priv_t *ctx,
 const uint8_t *data,
 unsigned int           data_sz,
 vpx_codec_err_t *res)
{
 *res = VPX_CODEC_OK;

 if (ctx->fragments.count == 0)
 {
  
        memset((void*)ctx->fragments.ptrs, 0, sizeof(ctx->fragments.ptrs));
        memset(ctx->fragments.sizes, 0, sizeof(ctx->fragments.sizes));
 }
 if (ctx->fragments.enabled && !(data == NULL && data_sz == 0))
 {
  
        ctx->fragments.ptrs[ctx->fragments.count] = data;
        ctx->fragments.sizes[ctx->fragments.count] = data_sz;
        ctx->fragments.count++;
 if (ctx->fragments.count > (1 << EIGHT_PARTITION) + 1)
 {
            ctx->fragments.count = 0;
 *res = VPX_CODEC_INVALID_PARAM;
 return -1;
 }
 return 0;
 }

 if (!ctx->fragments.enabled && (data == NULL && data_sz == 0))
 {
 return 0;
 }

 if (!ctx->fragments.enabled)
 {
        ctx->fragments.ptrs[0] = data;
        ctx->fragments.sizes[0] = data_sz;
        ctx->fragments.count = 1;
 }

 return 1;
}
