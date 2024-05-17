status_t BufferQueueConsumer::getReleasedBuffers(uint64_t *outSlotMask) {
    ATRACE_CALL();

 if (outSlotMask == NULL) {
        BQ_LOGE("getReleasedBuffers: outSlotMask may not be NULL");
 return BAD_VALUE;
 }

 Mutex::Autolock lock(mCore->mMutex);

 if (mCore->mIsAbandoned) {
        BQ_LOGE("getReleasedBuffers: BufferQueue has been abandoned");
 return NO_INIT;
 }

 uint64_t mask = 0;
 for (int s = 0; s < BufferQueueDefs::NUM_BUFFER_SLOTS; ++s) {
 if (!mSlots[s].mAcquireCalled) {
            mask |= (1ULL << s);
 }
 }

 BufferQueueCore::Fifo::iterator current(mCore->mQueue.begin());
 while (current != mCore->mQueue.end()) {
 if (current->mAcquireCalled) {
            mask &= ~(1ULL << current->mSlot);
 }
 ++current;
 }

    BQ_LOGV("getReleasedBuffers: returning mask %#" PRIx64, mask);
 *outSlotMask = mask;
 return NO_ERROR;
}
