media_status_t AMediaCodec_setParameters(
 AMediaCodec *mData, const AMediaFormat* params) {
 if (params == NULL || mData == NULL) {
 return AMEDIA_ERROR_INVALID_PARAMETER;
 }
    sp<AMessage> nativeParams;
 AMediaFormat_getFormat(params, &nativeParams);
    ALOGV("setParameters: %s", nativeParams->debugString(0).c_str());

 return translate_error(mData->mCodec->setParameters(nativeParams));
}
