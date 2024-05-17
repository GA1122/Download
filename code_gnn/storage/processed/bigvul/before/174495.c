status_t BufferQueueConsumer::acquireBuffer(BufferItem* outBuffer,
 nsecs_t expectedPresent) {
    ATRACE_CALL();
 Mutex::Autolock lock(mCore->mMutex);

 int numAcquiredBuffers = 0;
 for (int s = 0; s < BufferQueueDefs::NUM_BUFFER_SLOTS; ++s) {
 if (mSlots[s].mBufferState == BufferSlot::ACQUIRED) {
 ++numAcquiredBuffers;
 }
 }
 if (numAcquiredBuffers >= mCore->mMaxAcquiredBufferCount + 1) {
        BQ_LOGE("acquireBuffer: max acquired buffer count reached: %d (max %d)",
                numAcquiredBuffers, mCore->mMaxAcquiredBufferCount);
 return INVALID_OPERATION;
 }

 if (mCore->mQueue.empty()) {
 return NO_BUFFER_AVAILABLE;
 }

 BufferQueueCore::Fifo::iterator front(mCore->mQueue.begin());

 if (expectedPresent != 0) {
 const int MAX_REASONABLE_NSEC = 1000000000ULL;  


 while (mCore->mQueue.size() > 1 && !mCore->mQueue[0].mIsAutoTimestamp) {
 const BufferItem& bufferItem(mCore->mQueue[1]);
 nsecs_t desiredPresent = bufferItem.mTimestamp;
 if (desiredPresent < expectedPresent - MAX_REASONABLE_NSEC ||
                    desiredPresent > expectedPresent) {
                BQ_LOGV("acquireBuffer: nodrop desire=%" PRId64 " expect=%"
 PRId64 " (%" PRId64 ") now=%" PRId64,
                        desiredPresent, expectedPresent,
                        desiredPresent - expectedPresent,
                        systemTime(CLOCK_MONOTONIC));
 break;
 }

            BQ_LOGV("acquireBuffer: drop desire=%" PRId64 " expect=%" PRId64
 " size=%zu",
                    desiredPresent, expectedPresent, mCore->mQueue.size());
 if (mCore->stillTracking(front)) {
                mSlots[front->mSlot].mBufferState = BufferSlot::FREE;
 }
            mCore->mQueue.erase(front);
            front = mCore->mQueue.begin();
 }

 nsecs_t desiredPresent = front->mTimestamp;
 if (desiredPresent > expectedPresent &&
                desiredPresent < expectedPresent + MAX_REASONABLE_NSEC) {
            BQ_LOGV("acquireBuffer: defer desire=%" PRId64 " expect=%" PRId64
 " (%" PRId64 ") now=%" PRId64,
                    desiredPresent, expectedPresent,
                    desiredPresent - expectedPresent,
                    systemTime(CLOCK_MONOTONIC));
 return PRESENT_LATER;
 }

        BQ_LOGV("acquireBuffer: accept desire=%" PRId64 " expect=%" PRId64 " "
 "(%" PRId64 ") now=%" PRId64, desiredPresent, expectedPresent,
                desiredPresent - expectedPresent,
                systemTime(CLOCK_MONOTONIC));
 }

 int slot = front->mSlot;
 *outBuffer = *front;
    ATRACE_BUFFER_INDEX(slot);

    BQ_LOGV("acquireBuffer: acquiring { slot=%d/%" PRIu64 " buffer=%p }",
            slot, front->mFrameNumber, front->mGraphicBuffer->handle);
 if (mCore->stillTracking(front)) {
        mSlots[slot].mAcquireCalled = true;
        mSlots[slot].mNeedsCleanupOnRelease = false;
        mSlots[slot].mBufferState = BufferSlot::ACQUIRED;
        mSlots[slot].mFence = Fence::NO_FENCE;
 }

 if (outBuffer->mAcquireCalled) {
        outBuffer->mGraphicBuffer = NULL;
 }

    mCore->mQueue.erase(front);

    mCore->mDequeueCondition.broadcast();

    ATRACE_INT(mCore->mConsumerName.string(), mCore->mQueue.size());

 return NO_ERROR;
}
