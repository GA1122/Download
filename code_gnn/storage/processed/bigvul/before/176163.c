media_status_t AMediaCodec_createPersistentInputSurface(ANativeWindow **surface) {
 if (surface == NULL) {
 return AMEDIA_ERROR_INVALID_PARAMETER;
 }
 *surface = NULL;

    sp<PersistentSurface> ps = MediaCodec::CreatePersistentInputSurface();
 if (ps == NULL) {
 return AMEDIA_ERROR_UNKNOWN;
 }

    sp<IGraphicBufferProducer> igbp = ps->getBufferProducer();
 if (igbp == NULL) {
 return AMEDIA_ERROR_UNKNOWN;
 }

 *surface = new AMediaCodecPersistentSurface(igbp, ps);
 ANativeWindow_acquire(*surface);

 return AMEDIA_OK;
}
