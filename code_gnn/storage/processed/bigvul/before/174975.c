void CameraClient::stopRecording() {
    LOG1("stopRecording (pid %d)", getCallingPid());
 Mutex::Autolock lock(mLock);
 if (checkPidAndHardware() != NO_ERROR) return;

    disableMsgType(CAMERA_MSG_VIDEO_FRAME);
    mHardware->stopRecording();
    mCameraService->playSound(CameraService::SOUND_RECORDING);

    mPreviewBuffer.clear();
}
