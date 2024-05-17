void StreamingProcessor::releaseAllRecordingFramesLocked() {
    ATRACE_CALL();
 status_t res;

 if (mRecordingConsumer == 0) {
 return;
 }

    ALOGV("%s: Camera %d: Releasing all recording buffers", __FUNCTION__,
            mId);

 size_t releasedCount = 0;
 for (size_t itemIndex = 0; itemIndex < mRecordingBuffers.size(); itemIndex++) {
 const BufferItem item = mRecordingBuffers[itemIndex];
 if (item.mBuf != BufferItemConsumer::INVALID_BUFFER_SLOT) {
            res = mRecordingConsumer->releaseBuffer(mRecordingBuffers[itemIndex]);
 if (res != OK) {
                ALOGE("%s: Camera %d: Unable to free recording frame "
 "(buffer_handle_t: %p): %s (%d)", __FUNCTION__,
                        mId, item.mGraphicBuffer->handle, strerror(-res), res);
 }
            mRecordingBuffers.replaceAt(itemIndex);
            releasedCount++;
 }
 }

 if (releasedCount > 0) {
        ALOGW("%s: Camera %d: Force-freed %zu outstanding buffers "
 "from previous recording session", __FUNCTION__, mId, releasedCount);
        ALOGE_IF(releasedCount != mRecordingHeapCount - mRecordingHeapFree,
 "%s: Camera %d: Force-freed %zu buffers, but expected %zu",
            __FUNCTION__, mId, releasedCount, mRecordingHeapCount - mRecordingHeapFree);
 }

    mRecordingHeapHead = 0;
    mRecordingHeapFree = mRecordingHeapCount;
}
