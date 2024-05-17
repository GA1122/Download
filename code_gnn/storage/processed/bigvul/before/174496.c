status_t BufferQueueConsumer::attachBuffer(int* outSlot,
 const sp<android::GraphicBuffer>& buffer) {
    ATRACE_CALL();

 if (outSlot == NULL) {
        BQ_LOGE("attachBuffer(P): outSlot must not be NULL");
 return BAD_VALUE;
 } else if (buffer == NULL) {
        BQ_LOGE("attachBuffer(P): cannot attach NULL buffer");
 return BAD_VALUE;
 }

 Mutex::Autolock lock(mCore->mMutex);

 int numAcquiredBuffers = 0;
 int found = BufferQueueCore::INVALID_BUFFER_SLOT;
 for (int s = 0; s < BufferQueueDefs::NUM_BUFFER_SLOTS; ++s) {
 if (mSlots[s].mBufferState == BufferSlot::ACQUIRED) {
 ++numAcquiredBuffers;
 } else if (mSlots[s].mBufferState == BufferSlot::FREE) {
 if (found == BufferQueueCore::INVALID_BUFFER_SLOT ||
                    mSlots[s].mFrameNumber < mSlots[found].mFrameNumber) {
                found = s;
 }
 }
 }

 if (numAcquiredBuffers >= mCore->mMaxAcquiredBufferCount + 1) {
        BQ_LOGE("attachBuffer(P): max acquired buffer count reached: %d "
 "(max %d)", numAcquiredBuffers,
                mCore->mMaxAcquiredBufferCount);
 return INVALID_OPERATION;
 }
 if (found == BufferQueueCore::INVALID_BUFFER_SLOT) {
        BQ_LOGE("attachBuffer(P): could not find free buffer slot");
 return NO_MEMORY;
 }

 *outSlot = found;
    ATRACE_BUFFER_INDEX(*outSlot);
    BQ_LOGV("attachBuffer(C): returning slot %d", *outSlot);

    mSlots[*outSlot].mGraphicBuffer = buffer;
    mSlots[*outSlot].mBufferState = BufferSlot::ACQUIRED;
    mSlots[*outSlot].mAttachedByConsumer = true;
    mSlots[*outSlot].mNeedsCleanupOnRelease = false;
    mSlots[*outSlot].mFence = Fence::NO_FENCE;
    mSlots[*outSlot].mFrameNumber = 0;

    mSlots[*outSlot].mAcquireCalled = false;

 return NO_ERROR;
}
