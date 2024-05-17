void CameraClient::handlePostview(const sp<IMemory>& mem) {
    disableMsgType(CAMERA_MSG_POSTVIEW_FRAME);

    sp<ICameraClient> c = mRemoteCallback;
    mLock.unlock();
 if (c != 0) {
        c->dataCallback(CAMERA_MSG_POSTVIEW_FRAME, mem, NULL);
 }
}
