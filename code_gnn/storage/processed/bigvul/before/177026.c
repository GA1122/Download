status_t InputDispatcher::unregisterInputChannelLocked(const sp<InputChannel>& inputChannel,
 bool notify) {
 ssize_t connectionIndex = getConnectionIndexLocked(inputChannel);
 if (connectionIndex < 0) {
        ALOGW("Attempted to unregister already unregistered input channel '%s'",
                inputChannel->getName().string());
 return BAD_VALUE;
 }

    sp<Connection> connection = mConnectionsByFd.valueAt(connectionIndex);
    mConnectionsByFd.removeItemsAt(connectionIndex);

 if (connection->monitor) {
        removeMonitorChannelLocked(inputChannel);
 }

    mLooper->removeFd(inputChannel->getFd());

 nsecs_t currentTime = now();
    abortBrokenDispatchCycleLocked(currentTime, connection, notify);

    connection->status = Connection::STATUS_ZOMBIE;
 return OK;
}
