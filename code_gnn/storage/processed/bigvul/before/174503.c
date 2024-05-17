status_t BufferQueueConsumer::releaseBuffer(int slot, uint64_t frameNumber,
 const sp<Fence>& releaseFence, EGLDisplay eglDisplay,
 EGLSyncKHR eglFence) {
    ATRACE_CALL();
    ATRACE_BUFFER_INDEX(slot);

 if (slot < 0 || slot >= BufferQueueDefs::NUM_BUFFER_SLOTS ||
            releaseFence == NULL) {
 return BAD_VALUE;
 }

    sp<IProducerListener> listener;
 {  
 Mutex::Autolock lock(mCore->mMutex);

 if (frameNumber != mSlots[slot].mFrameNumber) {
 return STALE_BUFFER_SLOT;
 }

 BufferQueueCore::Fifo::iterator current(mCore->mQueue.begin());
 while (current != mCore->mQueue.end()) {
 if (current->mSlot == slot) {
                BQ_LOGE("releaseBuffer: buffer slot %d pending release is "
 "currently queued", slot);
 return BAD_VALUE;
 }
 ++current;
 }

 if (mSlots[slot].mBufferState == BufferSlot::ACQUIRED) {
            mSlots[slot].mEglDisplay = eglDisplay;
            mSlots[slot].mEglFence = eglFence;
            mSlots[slot].mFence = releaseFence;
            mSlots[slot].mBufferState = BufferSlot::FREE;
            listener = mCore->mConnectedProducerListener;
            BQ_LOGV("releaseBuffer: releasing slot %d", slot);
 } else if (mSlots[slot].mNeedsCleanupOnRelease) {
            BQ_LOGV("releaseBuffer: releasing a stale buffer slot %d "
 "(state = %d)", slot, mSlots[slot].mBufferState);
            mSlots[slot].mNeedsCleanupOnRelease = false;
 return STALE_BUFFER_SLOT;
 } else {
            BQ_LOGV("releaseBuffer: attempted to release buffer slot %d "
 "but its state was %d", slot, mSlots[slot].mBufferState);
 return BAD_VALUE;
 }

        mCore->mDequeueCondition.broadcast();
 }  

 if (listener != NULL) {
        listener->onBufferReleased();
 }

 return NO_ERROR;
}
