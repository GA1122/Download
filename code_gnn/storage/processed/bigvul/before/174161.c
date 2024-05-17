void OMX::CallbackDispatcher::post(const omx_message &msg, bool realTime) {
 Mutex::Autolock autoLock(mLock);

    mQueue.push_back(msg);
 if (realTime) {
        mQueueChanged.signal();
 }
}
