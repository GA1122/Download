InputDispatcher::Connection::Connection(const sp<InputChannel>& inputChannel,
 const sp<InputWindowHandle>& inputWindowHandle, bool monitor) :
        status(STATUS_NORMAL), inputChannel(inputChannel), inputWindowHandle(inputWindowHandle),
        monitor(monitor),
        inputPublisher(inputChannel), inputPublisherBlocked(false) {
}
