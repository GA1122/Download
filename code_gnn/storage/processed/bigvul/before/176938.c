void InputDispatcher::finishDispatchCycleLocked(nsecs_t currentTime,
 const sp<Connection>& connection, uint32_t seq, bool handled) {
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ finishDispatchCycle - seq=%u, handled=%s",
            connection->getInputChannelName(), seq, toString(handled));
#endif

    connection->inputPublisherBlocked = false;

 if (connection->status == Connection::STATUS_BROKEN
 || connection->status == Connection::STATUS_ZOMBIE) {
 return;
 }

    onDispatchCycleFinishedLocked(currentTime, connection, seq, handled);
}
