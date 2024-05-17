InputDispatcher::DispatchEntry* InputDispatcher::Connection::findWaitQueueEntry(uint32_t seq) {
 for (DispatchEntry* entry = waitQueue.head; entry != NULL; entry = entry->next) {
 if (entry->seq == seq) {
 return entry;
 }
 }
 return NULL;
}
