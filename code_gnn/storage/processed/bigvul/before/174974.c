void CameraClient::stopPreview() {
    LOG1("stopPreview (pid %d)", getCallingPid());
 Mutex::Autolock lock(mLock);
 if (checkPidAndHardware() != NO_ERROR) return;


    disableMsgType(CAMERA_MSG_PREVIEW_FRAME);
    mHardware->stopPreview();

    mPreviewBuffer.clear();
}
