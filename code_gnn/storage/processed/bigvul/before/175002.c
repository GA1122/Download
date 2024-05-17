void ProCamera2Client::detachDevice() {
 if (mDevice == 0) return;

    ALOGV("Camera %d: Stopping processors", mCameraId);

    mFrameProcessor->removeListener(FRAME_PROCESSOR_LISTENER_MIN_ID,
                                    FRAME_PROCESSOR_LISTENER_MAX_ID,
  this);
    mFrameProcessor->requestExit();
    ALOGV("Camera %d: Waiting for threads", mCameraId);
    mFrameProcessor->join();
    ALOGV("Camera %d: Disconnecting device", mCameraId);

 {
        mDevice->clearStreamingRequest();

 status_t code;
 if ((code = mDevice->waitUntilDrained()) != OK) {
            ALOGE("%s: waitUntilDrained failed with code 0x%x", __FUNCTION__,
                  code);
 }
 }

 Camera2ClientBase::detachDevice();
}
