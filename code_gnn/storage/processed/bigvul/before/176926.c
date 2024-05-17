void InputDispatcher::drainInboundQueueLocked() {
 while (! mInboundQueue.isEmpty()) {
 EventEntry* entry = mInboundQueue.dequeueAtHead();
        releaseInboundEventLocked(entry);
 }
    traceInboundQueueLengthLocked();
}
