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

 const size_t kMaxBufferSize = 64 * 1024 * 1024;
 if (max_size > kMaxBufferSize) {
        ALOGE("bogus max input size: %zu > %zu", max_size, kMaxBufferSize);
 return ERROR_MALFORMED;
 }
 if (max_size == 0) {
        ALOGE("zero max input size");
 return ERROR_MALFORMED;
 }

 const size_t kMaxBuffers = 8;
 const size_t buffers = min(kMaxBufferSize / max_size, kMaxBuffers);
    mGroup = new MediaBufferGroup(buffers, max_size);
    mSrcBuffer = new (std::nothrow) uint8_t[max_size];
 if (mSrcBuffer == NULL) {
 delete mGroup;
        mGroup = NULL;
 return ERROR_MALFORMED;
 }

    mStarted = true;

 return OK;
}
