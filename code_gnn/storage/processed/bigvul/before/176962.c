bool InputDispatcher::isStaleEventLocked(nsecs_t currentTime, EventEntry* entry) {
 return currentTime - entry->eventTime >= STALE_EVENT_TIMEOUT;
}
