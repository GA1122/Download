void InputDispatcher::onDispatchCycleFinishedLocked(
 nsecs_t currentTime, const sp<Connection>& connection, uint32_t seq, bool handled) {
 CommandEntry* commandEntry = postCommandLocked(
 & InputDispatcher::doDispatchCycleFinishedLockedInterruptible);
    commandEntry->connection = connection;
    commandEntry->eventTime = currentTime;
    commandEntry->seq = seq;
    commandEntry->handled = handled;
}
