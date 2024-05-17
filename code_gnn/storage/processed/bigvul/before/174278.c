status_t Camera3Device::tearDown(int streamId) {
    ATRACE_CALL();
    ALOGV("%s: Camera %d: Tearing down stream %d", __FUNCTION__, mId, streamId);
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);

 if (mHal3Device->common.version < CAMERA_DEVICE_API_VERSION_3_2) {
        ALOGE("%s: Unable to tear down streams on device HAL v%x",
                __FUNCTION__, mHal3Device->common.version);
 return NO_INIT;
 }

    sp<Camera3StreamInterface> stream;
 ssize_t outputStreamIdx = mOutputStreams.indexOfKey(streamId);
 if (outputStreamIdx == NAME_NOT_FOUND) {
        CLOGE("Stream %d does not exist", streamId);
 return BAD_VALUE;
 }

    stream = mOutputStreams.editValueAt(outputStreamIdx);

 if (stream->hasOutstandingBuffers() || mRequestThread->isStreamPending(stream)) {
        CLOGE("Stream %d is a target of a in-progress request", streamId);
 return BAD_VALUE;
 }

 return stream->tearDown();
}
