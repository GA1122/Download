status_t NuMediaExtractor::setDataSource(const sp<DataSource> &source) {
 Mutex::Autolock autoLock(mLock);

 if (mImpl != NULL) {
 return -EINVAL;
 }

 status_t err = source->initCheck();
 if (err != OK) {
 return err;
 }

    mImpl = MediaExtractor::Create(source);

 if (mImpl == NULL) {
 return ERROR_UNSUPPORTED;
 }

    err = updateDurationAndBitrate();
 if (err == OK) {
        mDataSource = source;
 }

 return err;
}
