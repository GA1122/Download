void CameraClient::handleGenericNotify(int32_t msgType,
 int32_t ext1, int32_t ext2) {
    sp<ICameraClient> c = mRemoteCallback;
    mLock.unlock();
 if (c != 0) {
        c->notifyCallback(msgType, ext1, ext2);
 }
}
