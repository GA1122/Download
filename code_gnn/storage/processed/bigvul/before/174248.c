status_t Camera3Device::PreparerThread::prepare(int maxCount, sp<Camera3StreamInterface>& stream) {
 status_t res;

 Mutex::Autolock l(mLock);

    res = stream->startPrepare(maxCount);
 if (res == OK) {
        ALOGV("%s: Stream %d already prepared", __FUNCTION__, stream->getId());
 if (mListener) {
            mListener->notifyPrepared(stream->getId());
 }
 return OK;
 } else if (res != NOT_ENOUGH_DATA) {
 return res;
 }

 if (!mActive) {
 Thread::requestExitAndWait();
        res = Thread::run("C3PrepThread", PRIORITY_BACKGROUND);
 if (res != OK) {
            ALOGE("%s: Unable to start preparer stream: %d (%s)", __FUNCTION__, res, strerror(-res));
 if (mListener) {
                mListener->notifyPrepared(stream->getId());
 }
 return res;
 }
        mCancelNow = false;
        mActive = true;
        ALOGV("%s: Preparer stream started", __FUNCTION__);
 }

    mPendingStreams.push_back(stream);
    ALOGV("%s: Stream %d queued for preparing", __FUNCTION__, stream->getId());

 return OK;
}
