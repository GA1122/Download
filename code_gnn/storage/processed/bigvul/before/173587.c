void NuPlayer::GenericSource::notifyPreparedAndCleanup(status_t err) {
 if (err != OK) {
 {
            sp<DataSource> dataSource = mDataSource;
            sp<NuCachedSource2> cachedSource = mCachedSource;
            sp<DataSource> httpSource = mHttpSource;
 {
 Mutex::Autolock _l(mDisconnectLock);
                mDataSource.clear();
                mDecryptHandle = NULL;
                mDrmManagerClient = NULL;
                mCachedSource.clear();
                mHttpSource.clear();
 }
 }
        mBitrate = -1;

        cancelPollBuffering();
 }
    notifyPrepared(err);
}
