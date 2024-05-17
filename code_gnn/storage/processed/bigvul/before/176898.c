void InputDispatcher::addRecentEventLocked(EventEntry* entry) {
    entry->refCount += 1;
    mRecentQueue.enqueueAtTail(entry);
 if (mRecentQueue.count() > RECENT_QUEUE_MAX_SIZE) {
        mRecentQueue.dequeueAtHead()->release();
 }
}
