media_status_t AMediaCodec_createInputSurface(AMediaCodec *mData, ANativeWindow **surface) {
 if (surface == NULL || mData == NULL) {
 return AMEDIA_ERROR_INVALID_PARAMETER;
 }
 *surface = NULL;

    sp<IGraphicBufferProducer> igbp = NULL;
 status_t err = mData->mCodec->createInputSurface(&igbp);
 if (err != NO_ERROR) {
 return translate_error(err);
 }

 *surface = new Surface(igbp);
 ANativeWindow_acquire(*surface);
 return AMEDIA_OK;
}
