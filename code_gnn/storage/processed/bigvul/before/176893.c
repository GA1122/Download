void InputDispatcher::abortBrokenDispatchCycleLocked(nsecs_t currentTime,
 const sp<Connection>& connection, bool notify) {
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ abortBrokenDispatchCycle - notify=%s",
            connection->getInputChannelName(), toString(notify));
#endif

    drainDispatchQueueLocked(&connection->outboundQueue);
    traceOutboundQueueLengthLocked(connection);
    drainDispatchQueueLocked(&connection->waitQueue);
    traceWaitQueueLengthLocked(connection);

 if (connection->status == Connection::STATUS_NORMAL) {
        connection->status = Connection::STATUS_BROKEN;

 if (notify) {
            onDispatchCycleBrokenLocked(currentTime, connection);
 }
 }
}
