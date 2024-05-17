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

    mGroup = new MediaBufferGroup;

 int32_t max_size;
    CHECK(mFormat->findInt32(kKeyMaxInputSize, &max_size));

    mGroup->add_buffer(new MediaBuffer(max_size));

    mSrcBuffer = new (std::nothrow) uint8_t[max_size];
 if (mSrcBuffer == NULL) {
 return ERROR_MALFORMED;
 }

    mStarted = true;

 return OK;
}
