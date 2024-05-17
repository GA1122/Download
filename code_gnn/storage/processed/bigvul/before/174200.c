status_t Camera3Device::PreparerThread::clear() {
 status_t res;

 Mutex::Autolock l(mLock);

 for (const auto& stream : mPendingStreams) {
        stream->cancelPrepare();
 }
    mPendingStreams.clear();
    mCancelNow = true;

 return OK;
}
