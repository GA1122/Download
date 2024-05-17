status_t NuMediaExtractor::updateDurationAndBitrate() {
 if (mImpl->countTracks() > kMaxTrackCount) {
 return ERROR_UNSUPPORTED;
 }

    mTotalBitrate = 0ll;
    mDurationUs = -1ll;

 for (size_t i = 0; i < mImpl->countTracks(); ++i) {
        sp<MetaData> meta = mImpl->getTrackMetaData(i);
 if (meta == NULL) {
            ALOGW("no metadata for track %zu", i);
 continue;
 }

 int32_t bitrate;
 if (!meta->findInt32(kKeyBitRate, &bitrate)) {
 const char *mime;
            CHECK(meta->findCString(kKeyMIMEType, &mime));
            ALOGV("track of type '%s' does not publish bitrate", mime);

            mTotalBitrate = -1ll;
 } else if (mTotalBitrate >= 0ll) {
            mTotalBitrate += bitrate;
 }

 int64_t durationUs;
 if (meta->findInt64(kKeyDuration, &durationUs)
 && durationUs > mDurationUs) {
            mDurationUs = durationUs;
 }
 }
 return OK;
}
