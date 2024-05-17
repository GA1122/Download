status_t MediaMetadataRetriever::setDataSource(
 const sp<IMediaHTTPService> &httpService,
 const char *srcUrl,
 const KeyedVector<String8, String8> *headers)
{
    ALOGV("setDataSource");
 Mutex::Autolock _l(mLock);
 if (mRetriever == 0) {
        ALOGE("retriever is not initialized");
 return INVALID_OPERATION;
 }
 if (srcUrl == NULL) {
        ALOGE("data source is a null pointer");
 return UNKNOWN_ERROR;
 }
    ALOGV("data source (%s)", srcUrl);
 return mRetriever->setDataSource(httpService, srcUrl, headers);
}
