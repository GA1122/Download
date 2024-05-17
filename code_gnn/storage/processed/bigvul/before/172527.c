status_t CameraSource::reset() {
    ALOGD("reset: E");

 {
 Mutex::Autolock autoLock(mLock);
        mStarted = false;
        mFrameAvailableCondition.signal();

 int64_t token;
 bool isTokenValid = false;
 if (mCamera != 0) {
            token = IPCThreadState::self()->clearCallingIdentity();
            isTokenValid = true;
 }
        releaseQueuedFrames();
 while (!mFramesBeingEncoded.empty()) {
 if (NO_ERROR !=
                mFrameCompleteCondition.waitRelative(mLock,
                        mTimeBetweenFrameCaptureUs * 1000LL + CAMERA_SOURCE_TIMEOUT_NS)) {
                ALOGW("Timed out waiting for outstanding frames being encoded: %zu",
                    mFramesBeingEncoded.size());
 }
 }
        stopCameraRecording();
 if (isTokenValid) {
 IPCThreadState::self()->restoreCallingIdentity(token);
 }

 if (mCollectStats) {
            ALOGI("Frames received/encoded/dropped: %d/%d/%d in %" PRId64 " us",
                    mNumFramesReceived, mNumFramesEncoded, mNumFramesDropped,
                    mLastFrameTimestampUs - mFirstFrameTimeUs);
 }

 if (mNumGlitches > 0) {
            ALOGW("%d long delays between neighboring video frames", mNumGlitches);
 }

        CHECK_EQ(mNumFramesReceived, mNumFramesEncoded + mNumFramesDropped);
 }

    releaseCamera();

    ALOGD("reset: X");
 return OK;
}
