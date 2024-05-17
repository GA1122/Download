 String8 InputDispatcher::checkWindowReadyForMoreInputLocked(nsecs_t currentTime,
         const sp<InputWindowHandle>& windowHandle, const EventEntry* eventEntry,
         const char* targetType) {
 if (windowHandle->getInfo()->paused) {
 return String8::format("Waiting because the %s window is paused.", targetType);
 }

 ssize_t connectionIndex = getConnectionIndexLocked(windowHandle->getInputChannel());
 if (connectionIndex < 0) {
 return String8::format("Waiting because the %s window's input channel is not "
 "registered with the input dispatcher.  The window may be in the process "
 "of being removed.", targetType);
 }

    sp<Connection> connection = mConnectionsByFd.valueAt(connectionIndex);
 if (connection->status != Connection::STATUS_NORMAL) {
 return String8::format("Waiting because the %s window's input connection is %s."
 "The window may be in the process of being removed.", targetType,
                connection->getStatusLabel());
 }

 if (connection->inputPublisherBlocked) {
 return String8::format("Waiting because the %s window's input channel is full.  "
 "Outbound queue length: %d.  Wait queue length: %d.",
                targetType, connection->outboundQueue.count(), connection->waitQueue.count());
 }

 if (eventEntry->type == EventEntry::TYPE_KEY) {
 if (!connection->outboundQueue.isEmpty() || !connection->waitQueue.isEmpty()) {
 return String8::format("Waiting to send key event because the %s window has not "
 "finished processing all of the input events that were previously "
 "delivered to it.  Outbound queue length: %d.  Wait queue length: %d.",
                    targetType, connection->outboundQueue.count(), connection->waitQueue.count());
 }
 } else {
 if (!connection->waitQueue.isEmpty()
 && currentTime >= connection->waitQueue.head->deliveryTime
 + STREAM_AHEAD_EVENT_TIMEOUT) {
 return String8::format("Waiting to send non-key event because the %s window has not "
 "finished processing certain input events that were delivered to it over "
 "%0.1fms ago.  Wait queue length: %d.  Wait queue head age: %0.1fms.",
                    targetType, STREAM_AHEAD_EVENT_TIMEOUT * 0.000001f,
                    connection->waitQueue.count(),
 (currentTime - connection->waitQueue.head->deliveryTime) * 0.000001f);
 }
 }
 return String8::empty();
}
