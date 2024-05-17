void NuPlayer::GenericSource::onPrepareAsync() {
 if (mDataSource == NULL) {
        mIsSecure = false;

 if (!mUri.empty()) {
 const char* uri = mUri.c_str();
 String8 contentType;
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
                   mHTTPService, uri, &mUriHeaders, &contentType,
 static_cast<HTTPBase *>(mHttpSource.get()));
 } else {
            mIsWidevine = false;

            mDataSource = new FileSource(mFd, mOffset, mLength);
            mFd = -1;
 }

 if (mDataSource == NULL) {
            ALOGE("Failed to create data source!");
            notifyPreparedAndCleanup(UNKNOWN_ERROR);
 return;
 }
 }

 if (mDataSource->flags() & DataSource::kIsCachingDataSource) {
        mCachedSource = static_cast<NuCachedSource2 *>(mDataSource.get());
 }

    mIsStreaming = (mIsWidevine || mCachedSource != NULL);

 status_t err = initFromDataSource();

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
 (mIsSecure ? FLAG_SECURE : 0)
 | (mDecryptHandle != NULL ? FLAG_PROTECTED : 0)
 | FLAG_CAN_PAUSE
 | FLAG_CAN_SEEK_BACKWARD
 | FLAG_CAN_SEEK_FORWARD
 | FLAG_CAN_SEEK);

 if (mIsSecure) {
        sp<AMessage> reply = new AMessage(kWhatSecureDecodersInstantiated, this);
        notifyInstantiateSecureDecoders(reply);
 } else {
        finishPrepareAsync();
 }
}
