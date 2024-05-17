SoftVPX::~SoftVPX() {
    vpx_codec_destroy((vpx_codec_ctx_t *)mCtx);
 delete (vpx_codec_ctx_t *)mCtx;
    mCtx = NULL;
}
