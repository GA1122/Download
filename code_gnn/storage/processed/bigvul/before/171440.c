static vpx_codec_err_t ctrl_set_decryptor(vpx_codec_alg_priv_t *ctx,
                                          va_list args) {
  vpx_decrypt_init *init = va_arg(args, vpx_decrypt_init *);
  ctx->decrypt_cb = init ? init->decrypt_cb : NULL;
  ctx->decrypt_state = init ? init->decrypt_state : NULL;
 return VPX_CODEC_OK;
}
