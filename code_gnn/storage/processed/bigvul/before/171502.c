status_t SoftVPX::destroyDecoder() {
    vpx_codec_destroy((vpx_codec_ctx_t *)mCtx);
 delete (vpx_codec_ctx_t *)mCtx;
    mCtx = NULL;
 return OK;
}
