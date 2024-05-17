status_t MPEG4Source::stop() {
 Mutex::Autolock autoLock(mLock);

    CHECK(mStarted);

 if (mBuffer != NULL) {
        mBuffer->release();
        mBuffer = NULL;
 }

 delete[] mSrcBuffer;
    mSrcBuffer = NULL;

 delete mGroup;
    mGroup = NULL;

    mStarted = false;
    mCurrentSampleIndex = 0;

 return OK;
}
