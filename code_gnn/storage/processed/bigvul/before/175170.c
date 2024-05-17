status_t NuPlayer::GenericSource::prefillCacheIfNecessary() {
    CHECK(mDataSource != NULL);

 if (mCachedSource == NULL) {
 return OK;
 }

 if (!strncasecmp(mContentType.string(), "audio/", 6)) {
 return OK;
 }


 static const size_t kMinBytesForSniffing = 192 * 1024;
 static const size_t kDefaultMetaSize = 200000;

 status_t finalStatus;

 size_t cachedDataRemaining =
            mCachedSource->approxDataRemaining(&finalStatus);

 if (finalStatus != OK || (mMetaDataSize >= 0
 && (off64_t)cachedDataRemaining >= mMetaDataSize)) {
        ALOGV("stop caching, status %d, "
 "metaDataSize %lld, cachedDataRemaining %zu",
                finalStatus, mMetaDataSize, cachedDataRemaining);
 return OK;
 }

    ALOGV("now cached %zu bytes of data", cachedDataRemaining);

 if (mMetaDataSize < 0
 && cachedDataRemaining >= kMinBytesForSniffing) {
 String8 tmp;
 float confidence;
        sp<AMessage> meta;
 if (!mCachedSource->sniff(&tmp, &confidence, &meta)) {
 return UNKNOWN_ERROR;
 }

        mSniffedMIME = tmp.string();

 if (meta == NULL
 || !meta->findInt64("meta-data-size",
 reinterpret_cast<int64_t*>(&mMetaDataSize))) {
            mMetaDataSize = kDefaultMetaSize;
 }

 if (mMetaDataSize < 0ll) {
            ALOGE("invalid metaDataSize = %lld bytes", mMetaDataSize);
 return UNKNOWN_ERROR;
 }
 }

 return -EAGAIN;
}
