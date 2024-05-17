status_t StreamingProcessor::setRecordingFormat(int format,
        android_dataspace dataSpace) {
    ATRACE_CALL();

 Mutex::Autolock m(mMutex);

    ALOGV("%s: Camera %d: New recording format/dataspace from encoder: %X, %X",
            __FUNCTION__, mId, format, dataSpace);

    mRecordingFormat = format;
    mRecordingDataSpace = dataSpace;
 int prevGrallocUsage = mRecordingGrallocUsage;
 if (mRecordingFormat == HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED) {
        mRecordingGrallocUsage = GRALLOC_USAGE_HW_VIDEO_ENCODER;
 } else {
        mRecordingGrallocUsage = GRALLOC_USAGE_SW_READ_OFTEN;
 }

    ALOGV("%s: Camera %d: New recording gralloc usage: %08X", __FUNCTION__, mId,
            mRecordingGrallocUsage);

 if (prevGrallocUsage != mRecordingGrallocUsage) {
        ALOGV("%s: Camera %d: Resetting recording consumer for new usage",
            __FUNCTION__, mId);

 if (isStreamActive(mActiveStreamIds, mRecordingStreamId)) {
            ALOGE("%s: Camera %d: Changing recording format when "
 "recording stream is already active!", __FUNCTION__,
                    mId);
 return INVALID_OPERATION;
 }

        releaseAllRecordingFramesLocked();

        mRecordingConsumer.clear();
 }

 return OK;
}
