status_t CameraSource::startCameraRecording() {
    ALOGV("startCameraRecording");
 int64_t token = IPCThreadState::self()->clearCallingIdentity();
 status_t err;
 if (mNumInputBuffers > 0) {
        err = mCamera->sendCommand(
            CAMERA_CMD_SET_VIDEO_BUFFER_COUNT, mNumInputBuffers, 0);

 if (err != OK) {
            ALOGW("Failed to set video buffer count to %d due to %d",
                mNumInputBuffers, err);
 }
 }

    err = mCamera->sendCommand(
        CAMERA_CMD_SET_VIDEO_FORMAT, mEncoderFormat, mEncoderDataSpace);

 if (err != OK) {
        ALOGW("Failed to set video encoder format/dataspace to %d, %d due to %d",
                mEncoderFormat, mEncoderDataSpace, err);
 }

    err = OK;
 if (mCameraFlags & FLAGS_HOT_CAMERA) {
        mCamera->unlock();
        mCamera.clear();
 if ((err = mCameraRecordingProxy->startRecording(
 new ProxyListener(this))) != OK) {
            ALOGE("Failed to start recording, received error: %s (%d)",
                    strerror(-err), err);
 }
 } else {
        mCamera->setListener(new CameraSourceListener(this));
        mCamera->startRecording();
 if (!mCamera->recordingEnabled()) {
            err = -EINVAL;
            ALOGE("Failed to start recording");
 }
 }
 IPCThreadState::self()->restoreCallingIdentity(token);
 return err;
}
