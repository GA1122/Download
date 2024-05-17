status_t MPEG4Source::start(MetaData *params) {
 Mutex::Autolock autoLock(mLock);

    CHECK(!mStarted);

 int32_t val;
 if (params && params->findInt32(kKeyWantsNALFragments, &val)
 && val != 0) {
        mWantsNALFragments = true;
 } else {
        mWantsNALFragments = false;
 }

 int32_t tmp;
    CHECK(mFormat->findInt32(kKeyMaxInputSize, &tmp));
 size_t max_size = tmp;

 if (max_size > 64 * 1024 * 1024) {
        ALOGE("bogus max input size: %zu", max_size);
 return ERROR_MALFORMED;
 }
    mGroup = new MediaBufferGroup;
    mGroup->add_buffer(new MediaBuffer(max_size));

    mSrcBuffer = new (std::nothrow) uint8_t[max_size];
 if (mSrcBuffer == NULL) {
 delete mGroup;
        mGroup = NULL;
 return ERROR_MALFORMED;
 }

    mStarted = true;

 return OK;
}
