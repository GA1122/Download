static vpx_codec_err_t vp8_set_decryptor(vpx_codec_alg_priv_t *ctx,
                                         va_list args)
{
    vpx_decrypt_init *init = va_arg(args, vpx_decrypt_init *);

 if (init)
 {
        ctx->decrypt_cb = init->decrypt_cb;
        ctx->decrypt_state = init->decrypt_state;
 }
 else
 {
        ctx->decrypt_cb = NULL;
        ctx->decrypt_state = NULL;
 }
 return VPX_CODEC_OK;
}
