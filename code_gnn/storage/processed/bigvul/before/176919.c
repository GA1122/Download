void InputDispatcher::doDispatchCycleFinishedLockedInterruptible(
 CommandEntry* commandEntry) {
    sp<Connection> connection = commandEntry->connection;
 nsecs_t finishTime = commandEntry->eventTime;
 uint32_t seq = commandEntry->seq;
 bool handled = commandEntry->handled;

 DispatchEntry* dispatchEntry = connection->findWaitQueueEntry(seq);
 if (dispatchEntry) {
 nsecs_t eventDuration = finishTime - dispatchEntry->deliveryTime;
 if (eventDuration > SLOW_EVENT_PROCESSING_WARNING_TIMEOUT) {
 String8 msg;
            msg.appendFormat("Window '%s' spent %0.1fms processing the last input event: ",
                    connection->getWindowName(), eventDuration * 0.000001f);
            dispatchEntry->eventEntry->appendDescription(msg);
            ALOGI("%s", msg.string());
 }

 bool restartEvent;
 if (dispatchEntry->eventEntry->type == EventEntry::TYPE_KEY) {
 KeyEntry* keyEntry = static_cast<KeyEntry*>(dispatchEntry->eventEntry);
            restartEvent = afterKeyEventLockedInterruptible(connection,
                    dispatchEntry, keyEntry, handled);
 } else if (dispatchEntry->eventEntry->type == EventEntry::TYPE_MOTION) {
 MotionEntry* motionEntry = static_cast<MotionEntry*>(dispatchEntry->eventEntry);
            restartEvent = afterMotionEventLockedInterruptible(connection,
                    dispatchEntry, motionEntry, handled);
 } else {
            restartEvent = false;
 }

 if (dispatchEntry == connection->findWaitQueueEntry(seq)) {
            connection->waitQueue.dequeue(dispatchEntry);
            traceWaitQueueLengthLocked(connection);
 if (restartEvent && connection->status == Connection::STATUS_NORMAL) {
                connection->outboundQueue.enqueueAtHead(dispatchEntry);
                traceOutboundQueueLengthLocked(connection);
 } else {
                releaseDispatchEntryLocked(dispatchEntry);
 }
 }

        startDispatchCycleLocked(now(), connection);
 }
}
