void NuPlayer::GenericSource::onPrepareAsync() {
 if (mDataSource == NULL) {
 if (!mUri.empty()) {
 const char* uri = mUri.c_str();
            mIsWidevine = !strncasecmp(uri, "widevine://", 11);

 if (!strncasecmp("http://", uri, 7)
 || !strncasecmp("https://", uri, 8)
 || mIsWidevine) {
                mHttpSource = DataSource::CreateMediaHTTP(mHTTPService);
 if (mHttpSource == NULL) {
                    ALOGE("Failed to create http source!");
                    notifyPreparedAndCleanup(UNKNOWN_ERROR);
 return;
 }
 }

            mDataSource = DataSource::CreateFromURI(
                   mHTTPService, uri, &mUriHeaders, &mContentType,
 static_cast<HTTPBase *>(mHttpSource.get()));
 } else {
            mIsWidevine = false;

            mDataSource = new FileSource(mFd, mOffset, mLength);
 }

 if (mDataSource == NULL) {
            ALOGE("Failed to create data source!");
            notifyPreparedAndCleanup(UNKNOWN_ERROR);
 return;
 }

 if (mDataSource->flags() & DataSource::kIsCachingDataSource) {
            mCachedSource = static_cast<NuCachedSource2 *>(mDataSource.get());
 }

 if (mIsWidevine || mCachedSource != NULL) {
            schedulePollBuffering();
 }
 }

 status_t err = prefillCacheIfNecessary();
 if (err != OK) {
 if (err == -EAGAIN) {
 (new AMessage(kWhatPrepareAsync, id()))->post(200000);
 } else {
            ALOGE("Failed to prefill data cache!");
            notifyPreparedAndCleanup(UNKNOWN_ERROR);
 }
 return;
 }

    err = initFromDataSource();

 if (err != OK) {
        ALOGE("Failed to init from data source!");
        notifyPreparedAndCleanup(err);
 return;
 }

 if (mVideoTrack.mSource != NULL) {
        sp<MetaData> meta = doGetFormatMeta(false  );
        sp<AMessage> msg = new AMessage;
        err = convertMetaDataToMessage(meta, &msg);
 if(err != OK) {
            notifyPreparedAndCleanup(err);
 return;
 }
        notifyVideoSizeChanged(msg);
 }

    notifyFlagsChanged(
 (mIsWidevine ? FLAG_SECURE : 0)
 | FLAG_CAN_PAUSE
 | FLAG_CAN_SEEK_BACKWARD
 | FLAG_CAN_SEEK_FORWARD
 | FLAG_CAN_SEEK);

    notifyPrepared();
}
