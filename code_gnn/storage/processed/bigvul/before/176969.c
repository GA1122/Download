void InputDispatcher::monitor() {
    mLock.lock();
    mLooper->wake();
    mDispatcherIsAliveCondition.wait(mLock);
    mLock.unlock();
}
