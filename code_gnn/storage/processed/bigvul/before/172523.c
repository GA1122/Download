status_t CameraSource::read(
 MediaBuffer **buffer, const ReadOptions *options) {
    ALOGV("read");

 *buffer = NULL;

 int64_t seekTimeUs;
 ReadOptions::SeekMode mode;
 if (options && options->getSeekTo(&seekTimeUs, &mode)) {
 return ERROR_UNSUPPORTED;
 }

    sp<IMemory> frame;
 int64_t frameTime;

 {
 Mutex::Autolock autoLock(mLock);
 while (mStarted && mFramesReceived.empty()) {
 if (NO_ERROR !=
                mFrameAvailableCondition.waitRelative(mLock,
                    mTimeBetweenFrameCaptureUs * 1000LL + CAMERA_SOURCE_TIMEOUT_NS)) {
 if (mCameraRecordingProxy != 0 &&
 !IInterface::asBinder(mCameraRecordingProxy)->isBinderAlive()) {
                    ALOGW("camera recording proxy is gone");
 return ERROR_END_OF_STREAM;
 }
                ALOGW("Timed out waiting for incoming camera video frames: %" PRId64 " us",
                    mLastFrameTimestampUs);
 }
 }
 if (!mStarted) {
 return OK;
 }
        frame = *mFramesReceived.begin();
        mFramesReceived.erase(mFramesReceived.begin());

        frameTime = *mFrameTimes.begin();
        mFrameTimes.erase(mFrameTimes.begin());
        mFramesBeingEncoded.push_back(frame);
 *buffer = new MediaBuffer(frame->pointer(), frame->size());
 (*buffer)->setObserver(this);
 (*buffer)->add_ref();
 (*buffer)->meta_data()->setInt64(kKeyTime, frameTime);
 }
 return OK;
}
