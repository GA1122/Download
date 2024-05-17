void Camera2Client::stopRecording() {
    ATRACE_CALL();
    ALOGV("%s: E", __FUNCTION__);
 Mutex::Autolock icl(mBinderSerializationLock);
 SharedParameters::Lock l(mParameters);

 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return;

 switch (l.mParameters.state) {
 case Parameters::RECORD:
 break;
 case Parameters::STOPPED:
 case Parameters::PREVIEW:
 case Parameters::STILL_CAPTURE:
 case Parameters::VIDEO_SNAPSHOT:
 default:
            ALOGE("%s: Camera %d: Can't stop recording in state %s",
                    __FUNCTION__, mCameraId,
 Parameters::getStateName(l.mParameters.state));
 return;
 };

    mCameraService->playSound(CameraService::SOUND_RECORDING);

    res = startPreviewL(l.mParameters, true);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to return to preview",
                __FUNCTION__, mCameraId);
 }
}
