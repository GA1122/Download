void InputDispatcher::traceWaitQueueLengthLocked(const sp<Connection>& connection) {
 if (ATRACE_ENABLED()) {
 char counterName[40];
        snprintf(counterName, sizeof(counterName), "wq:%s", connection->getWindowName());
        ATRACE_INT(counterName, connection->waitQueue.count());
 }
}
