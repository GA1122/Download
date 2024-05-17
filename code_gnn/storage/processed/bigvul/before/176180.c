media_status_t AMediaCodec_setInputSurface(
 AMediaCodec *mData, ANativeWindow *surface) {

 if (surface == NULL || mData == NULL) {
 return AMEDIA_ERROR_INVALID_PARAMETER;
 }

 AMediaCodecPersistentSurface *aMediaPersistentSurface =
 static_cast<AMediaCodecPersistentSurface *>(surface);
 if (aMediaPersistentSurface->mPersistentSurface == NULL) {
 return AMEDIA_ERROR_INVALID_PARAMETER;
 }

 return translate_error(mData->mCodec->setInputSurface(
            aMediaPersistentSurface->mPersistentSurface));
}
