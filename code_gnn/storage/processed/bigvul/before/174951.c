void CameraClient::handleGenericDataTimestamp(nsecs_t timestamp,
 int32_t msgType, const sp<IMemory>& dataPtr) {
    sp<ICameraClient> c = mRemoteCallback;
    mLock.unlock();
 if (c != 0) {
        c->dataCallbackTimestamp(timestamp, msgType, dataPtr);
 }
}
