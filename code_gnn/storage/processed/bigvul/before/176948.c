int InputDispatcher::handleReceiveCallback(int fd, int events, void* data) {
 InputDispatcher* d = static_cast<InputDispatcher*>(data);

 {  
 AutoMutex _l(d->mLock);

 ssize_t connectionIndex = d->mConnectionsByFd.indexOfKey(fd);
 if (connectionIndex < 0) {
            ALOGE("Received spurious receive callback for unknown input channel.  "
 "fd=%d, events=0x%x", fd, events);
 return 0;  
 }

 bool notify;
        sp<Connection> connection = d->mConnectionsByFd.valueAt(connectionIndex);
 if (!(events & (ALOOPER_EVENT_ERROR | ALOOPER_EVENT_HANGUP))) {
 if (!(events & ALOOPER_EVENT_INPUT)) {
                ALOGW("channel '%s' ~ Received spurious callback for unhandled poll event.  "
 "events=0x%x", connection->getInputChannelName(), events);
 return 1;
 }

 nsecs_t currentTime = now();
 bool gotOne = false;
 status_t status;
 for (;;) {
 uint32_t seq;
 bool handled;
                status = connection->inputPublisher.receiveFinishedSignal(&seq, &handled);
 if (status) {
 break;
 }
                d->finishDispatchCycleLocked(currentTime, connection, seq, handled);
                gotOne = true;
 }
 if (gotOne) {
                d->runCommandsLockedInterruptible();
 if (status == WOULD_BLOCK) {
 return 1;
 }
 }

            notify = status != DEAD_OBJECT || !connection->monitor;
 if (notify) {
                ALOGE("channel '%s' ~ Failed to receive finished signal.  status=%d",
                        connection->getInputChannelName(), status);
 }
 } else {
            notify = !connection->monitor;
 if (notify) {
                ALOGW("channel '%s' ~ Consumer closed input channel or an error occurred.  "
 "events=0x%x", connection->getInputChannelName(), events);
 }
 }

        d->unregisterInputChannelLocked(connection->inputChannel, notify);
 return 0;  
 }  
}
