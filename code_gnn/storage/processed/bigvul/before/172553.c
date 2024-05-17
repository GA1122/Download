status_t StreamingProcessor::updateRecordingStream(const Parameters &params) {
    ATRACE_CALL();
 status_t res;
 Mutex::Autolock m(mMutex);

    sp<CameraDeviceBase> device = mDevice.promote();
 if (device == 0) {
        ALOGE("%s: Camera %d: Device does not exist", __FUNCTION__, mId);
 return INVALID_OPERATION;
 }

 bool newConsumer = false;
 if (mRecordingConsumer == 0) {
        ALOGV("%s: Camera %d: Creating recording consumer with %zu + 1 "
 "consumer-side buffers", __FUNCTION__, mId, mRecordingHeapCount);
        sp<IGraphicBufferProducer> producer;
        sp<IGraphicBufferConsumer> consumer;
 BufferQueue::createBufferQueue(&producer, &consumer);
        mRecordingConsumer = new BufferItemConsumer(consumer,
                mRecordingGrallocUsage,
                mRecordingHeapCount + 1);
        mRecordingConsumer->setFrameAvailableListener(this);
        mRecordingConsumer->setName(String8("Camera2-RecordingConsumer"));
        mRecordingWindow = new Surface(producer);
        newConsumer = true;
 }

 if (mRecordingStreamId != NO_STREAM) {
 uint32_t currentWidth, currentHeight;
 uint32_t currentFormat;
        android_dataspace currentDataSpace;
        res = device->getStreamInfo(mRecordingStreamId,
 &currentWidth, &currentHeight,
 &currentFormat, &currentDataSpace);
 if (res != OK) {
            ALOGE("%s: Camera %d: Error querying recording output stream info: "
 "%s (%d)", __FUNCTION__, mId,
                    strerror(-res), res);
 return res;
 }
 if (currentWidth != (uint32_t)params.videoWidth ||
                currentHeight != (uint32_t)params.videoHeight ||
                currentFormat != (uint32_t)mRecordingFormat ||
                currentDataSpace != mRecordingDataSpace ||
                newConsumer) {
            res = device->deleteStream(mRecordingStreamId);

 if (res == -EBUSY) {
                ALOGV("%s: Camera %d: Device is busy, call "
 "updateRecordingStream after it becomes idle",
                      __FUNCTION__, mId);
 return res;
 } else if (res != OK) {
                ALOGE("%s: Camera %d: Unable to delete old output stream "
 "for recording: %s (%d)", __FUNCTION__,
                        mId, strerror(-res), res);
 return res;
 }
            mRecordingStreamId = NO_STREAM;
 }
 }

 if (mRecordingStreamId == NO_STREAM) {
        mRecordingFrameCount = 0;
        res = device->createStream(mRecordingWindow,
                params.videoWidth, params.videoHeight,
                mRecordingFormat, mRecordingDataSpace,
                CAMERA3_STREAM_ROTATION_0, &mRecordingStreamId);
 if (res != OK) {
            ALOGE("%s: Camera %d: Can't create output stream for recording: "
 "%s (%d)", __FUNCTION__, mId,
                    strerror(-res), res);
 return res;
 }
 }

 return OK;
}
