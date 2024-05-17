void NuPlayer::GenericSource::disconnect() {
    sp<DataSource> dataSource, httpSource;
 {
 Mutex::Autolock _l(mDisconnectLock);
        dataSource = mDataSource;
        httpSource = mHttpSource;
 }

 if (dataSource != NULL) {
 if (dataSource->flags() & DataSource::kIsCachingDataSource) {
 static_cast<NuCachedSource2 *>(dataSource.get())->disconnect();
 }
 } else if (httpSource != NULL) {
 static_cast<HTTPBase *>(httpSource.get())->disconnect();
 }
}
