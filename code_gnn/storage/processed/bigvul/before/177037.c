InputDispatcher::~InputDispatcher() {
 {  
 AutoMutex _l(mLock);

        resetKeyRepeatLocked();
        releasePendingEventLocked();
        drainInboundQueueLocked();
 }

 while (mConnectionsByFd.size() != 0) {
        unregisterInputChannel(mConnectionsByFd.valueAt(0)->inputChannel);
 }
}
