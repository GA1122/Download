void InputDispatcher::doNotifyANRLockedInterruptible(
 CommandEntry* commandEntry) {
    mLock.unlock();

 nsecs_t newTimeout = mPolicy->notifyANR(
            commandEntry->inputApplicationHandle, commandEntry->inputWindowHandle,
            commandEntry->reason);

    mLock.lock();

    resumeAfterTargetsNotReadyTimeoutLocked(newTimeout,
            commandEntry->inputWindowHandle != NULL
 ? commandEntry->inputWindowHandle->getInputChannel() : NULL);
}
