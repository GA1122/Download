void InputDispatcher::traceOutboundQueueLengthLocked(const sp<Connection>& connection) {
 if (ATRACE_ENABLED()) {
 char counterName[40];
        snprintf(counterName, sizeof(counterName), "oq:%s", connection->getWindowName());
        ATRACE_INT(counterName, connection->outboundQueue.count());
 }
}
