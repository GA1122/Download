status_t InputDispatcher::registerInputChannel(const sp<InputChannel>& inputChannel,
 const sp<InputWindowHandle>& inputWindowHandle, bool monitor) {
#if DEBUG_REGISTRATION
    ALOGD("channel '%s' ~ registerInputChannel - monitor=%s", inputChannel->getName().string(),
            toString(monitor));
#endif

 {  
 AutoMutex _l(mLock);

 if (getConnectionIndexLocked(inputChannel) >= 0) {
            ALOGW("Attempted to register already registered input channel '%s'",
                    inputChannel->getName().string());
 return BAD_VALUE;
 }

        sp<Connection> connection = new Connection(inputChannel, inputWindowHandle, monitor);

 int fd = inputChannel->getFd();
        mConnectionsByFd.add(fd, connection);

 if (monitor) {
            mMonitoringChannels.push(inputChannel);
 }

        mLooper->addFd(fd, 0, ALOOPER_EVENT_INPUT, handleReceiveCallback, this);
 }  

    mLooper->wake();
 return OK;
}
