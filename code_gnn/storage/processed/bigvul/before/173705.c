status_t AudioSource::reset() {
 Mutex::Autolock autoLock(mLock);
 if (!mStarted) {
 return UNKNOWN_ERROR;
 }

 if (mInitCheck != OK) {
 return NO_INIT;
 }

    mStarted = false;
    mFrameAvailableCondition.signal();

    mRecord->stop();
    waitOutstandingEncodingFrames_l();
    releaseQueuedFrames_l();

 return OK;
}
