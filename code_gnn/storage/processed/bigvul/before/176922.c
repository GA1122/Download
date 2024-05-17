void InputDispatcher::doNotifyConfigurationChangedInterruptible(
 CommandEntry* commandEntry) {
    mLock.unlock();

    mPolicy->notifyConfigurationChanged(commandEntry->eventTime);

    mLock.lock();
}
