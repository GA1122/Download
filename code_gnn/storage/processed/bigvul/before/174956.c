void CameraClient::handleShutter(void) {
 if (mPlayShutterSound) {
        mCameraService->playSound(CameraService::SOUND_SHUTTER);
 }

    sp<ICameraClient> c = mRemoteCallback;
 if (c != 0) {
        mLock.unlock();
        c->notifyCallback(CAMERA_MSG_SHUTTER, 0, 0);
 if (!lockIfMessageWanted(CAMERA_MSG_SHUTTER)) return;
 }
    disableMsgType(CAMERA_MSG_SHUTTER);

    mLock.unlock();
}
