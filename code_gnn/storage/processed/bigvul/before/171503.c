status_t SoftVPX::initDecoder() {
    mCtx = new vpx_codec_ctx_t;
 vpx_codec_err_t vpx_err;
 vpx_codec_dec_cfg_t cfg;
 vpx_codec_flags_t flags;
    memset(&cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    memset(&flags, 0, sizeof(vpx_codec_flags_t));
    cfg.threads = GetCPUCoreCount();

 if (mFrameParallelMode) {
        flags |= VPX_CODEC_USE_FRAME_THREADING;
 }

 if ((vpx_err = vpx_codec_dec_init(
 (vpx_codec_ctx_t *)mCtx,
                 mMode == MODE_VP8 ? &vpx_codec_vp8_dx_algo : &vpx_codec_vp9_dx_algo,
 &cfg, flags))) {
        ALOGE("on2 decoder failed to initialize. (%d)", vpx_err);
 return UNKNOWN_ERROR;
 }

 return OK;
}
