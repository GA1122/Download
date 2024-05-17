void InputDispatcher::doNotifyInputChannelBrokenLockedInterruptible(
 CommandEntry* commandEntry) {
    sp<Connection> connection = commandEntry->connection;

 if (connection->status != Connection::STATUS_ZOMBIE) {
        mLock.unlock();

        mPolicy->notifyInputChannelBroken(connection->inputWindowHandle);

        mLock.lock();
 }
}
