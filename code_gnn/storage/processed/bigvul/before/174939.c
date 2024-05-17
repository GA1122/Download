status_t CameraClient::connect(const sp<ICameraClient>& client) {
 int callingPid = getCallingPid();
    LOG1("connect E (pid %d)", callingPid);
 Mutex::Autolock lock(mLock);

 if (mClientPid != 0 && checkPid() != NO_ERROR) {
        ALOGW("Tried to connect to a locked camera (old pid %d, new pid %d)",
                mClientPid, callingPid);
 return EBUSY;
 }

 if (mRemoteCallback != 0 &&
 (client->asBinder() == mRemoteCallback->asBinder())) {
        LOG1("Connect to the same client");
 return NO_ERROR;
 }

    mPreviewCallbackFlag = CAMERA_FRAME_CALLBACK_FLAG_NOOP;
    mClientPid = callingPid;
    mRemoteCallback = client;

    LOG1("connect X (pid %d)", callingPid);
 return NO_ERROR;
}
