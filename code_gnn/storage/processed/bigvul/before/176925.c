void InputDispatcher::drainDispatchQueueLocked(Queue<DispatchEntry>* queue) {
 while (!queue->isEmpty()) {
 DispatchEntry* dispatchEntry = queue->dequeueAtHead();
        releaseDispatchEntryLocked(dispatchEntry);
 }
}
