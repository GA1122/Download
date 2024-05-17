bool SniffDRM(
 const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *) {
    sp<DecryptHandle> decryptHandle = source->DrmInitialization();

 if (decryptHandle != NULL) {
 if (decryptHandle->decryptApiType == DecryptApiType::CONTAINER_BASED) {
 *mimeType = String8("drm+container_based+") + decryptHandle->mimeType;
 *confidence = 10.0f;
 } else if (decryptHandle->decryptApiType == DecryptApiType::ELEMENTARY_STREAM_BASED) {
 *mimeType = String8("drm+es_based+") + decryptHandle->mimeType;
 *confidence = 10.0f;
 } else {
 return false;
 }

 return true;
 }

 return false;
}
