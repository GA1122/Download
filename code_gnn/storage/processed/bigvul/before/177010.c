void InputDispatcher::startDispatchCycleLocked(nsecs_t currentTime,
 const sp<Connection>& connection) {
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ startDispatchCycle",
            connection->getInputChannelName());
#endif

 while (connection->status == Connection::STATUS_NORMAL
 && !connection->outboundQueue.isEmpty()) {
 DispatchEntry* dispatchEntry = connection->outboundQueue.head;
        dispatchEntry->deliveryTime = currentTime;

 status_t status;
 EventEntry* eventEntry = dispatchEntry->eventEntry;
 switch (eventEntry->type) {
 case EventEntry::TYPE_KEY: {
 KeyEntry* keyEntry = static_cast<KeyEntry*>(eventEntry);

            status = connection->inputPublisher.publishKeyEvent(dispatchEntry->seq,
                    keyEntry->deviceId, keyEntry->source,
                    dispatchEntry->resolvedAction, dispatchEntry->resolvedFlags,
                    keyEntry->keyCode, keyEntry->scanCode,
                    keyEntry->metaState, keyEntry->repeatCount, keyEntry->downTime,
                    keyEntry->eventTime);
 break;
 }

 case EventEntry::TYPE_MOTION: {
 MotionEntry* motionEntry = static_cast<MotionEntry*>(eventEntry);

 PointerCoords scaledCoords[MAX_POINTERS];
 const PointerCoords* usingCoords = motionEntry->pointerCoords;

 float xOffset, yOffset, scaleFactor;
 if ((motionEntry->source & AINPUT_SOURCE_CLASS_POINTER)
 && !(dispatchEntry->targetFlags & InputTarget::FLAG_ZERO_COORDS)) {
                scaleFactor = dispatchEntry->scaleFactor;
                xOffset = dispatchEntry->xOffset * scaleFactor;
                yOffset = dispatchEntry->yOffset * scaleFactor;
 if (scaleFactor != 1.0f) {
 for (uint32_t i = 0; i < motionEntry->pointerCount; i++) {
                        scaledCoords[i] = motionEntry->pointerCoords[i];
                        scaledCoords[i].scale(scaleFactor);
 }
                    usingCoords = scaledCoords;
 }
 } else {
                xOffset = 0.0f;
                yOffset = 0.0f;
                scaleFactor = 1.0f;

 if (dispatchEntry->targetFlags & InputTarget::FLAG_ZERO_COORDS) {
 for (uint32_t i = 0; i < motionEntry->pointerCount; i++) {
                        scaledCoords[i].clear();
 }
                    usingCoords = scaledCoords;
 }
 }

            status = connection->inputPublisher.publishMotionEvent(dispatchEntry->seq,
                    motionEntry->deviceId, motionEntry->source,
                    dispatchEntry->resolvedAction, motionEntry->actionButton,
                    dispatchEntry->resolvedFlags, motionEntry->edgeFlags,
                    motionEntry->metaState, motionEntry->buttonState,
                    xOffset, yOffset, motionEntry->xPrecision, motionEntry->yPrecision,
                    motionEntry->downTime, motionEntry->eventTime,
                    motionEntry->pointerCount, motionEntry->pointerProperties,
                    usingCoords);
 break;
 }

 default:
            ALOG_ASSERT(false);
 return;
 }

 if (status) {
 if (status == WOULD_BLOCK) {
 if (connection->waitQueue.isEmpty()) {
                    ALOGE("channel '%s' ~ Could not publish event because the pipe is full. "
 "This is unexpected because the wait queue is empty, so the pipe "
 "should be empty and we shouldn't have any problems writing an "
 "event to it, status=%d", connection->getInputChannelName(), status);
                    abortBrokenDispatchCycleLocked(currentTime, connection, true  );
 } else {
#if DEBUG_DISPATCH_CYCLE
                    ALOGD("channel '%s' ~ Could not publish event because the pipe is full, "
 "waiting for the application to catch up",
                            connection->getInputChannelName());
#endif
                    connection->inputPublisherBlocked = true;
 }
 } else {
                ALOGE("channel '%s' ~ Could not publish event due to an unexpected error, "
 "status=%d", connection->getInputChannelName(), status);
                abortBrokenDispatchCycleLocked(currentTime, connection, true  );
 }
 return;
 }

        connection->outboundQueue.dequeue(dispatchEntry);
        traceOutboundQueueLengthLocked(connection);
        connection->waitQueue.enqueueAtTail(dispatchEntry);
        traceWaitQueueLengthLocked(connection);
 }
}
