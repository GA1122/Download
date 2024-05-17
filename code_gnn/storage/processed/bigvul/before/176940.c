ssize_t InputDispatcher::getConnectionIndexLocked(const sp<InputChannel>& inputChannel) {
 ssize_t connectionIndex = mConnectionsByFd.indexOfKey(inputChannel->getFd());
 if (connectionIndex >= 0) {
        sp<Connection> connection = mConnectionsByFd.valueAt(connectionIndex);
 if (connection->inputChannel.get() == inputChannel.get()) {
 return connectionIndex;
 }
 }

 return -1;
}
