OMX::CallbackDispatcher::~CallbackDispatcher() {
 {
 Mutex::Autolock autoLock(mLock);

        mDone = true;
        mQueueChanged.signal();
 }

 status_t status = mThread->join();
 if (status != WOULD_BLOCK) {
        CHECK_EQ(status, (status_t)NO_ERROR);
 }
}
