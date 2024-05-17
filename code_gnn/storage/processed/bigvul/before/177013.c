void InputDispatcher::synthesizeCancelationEventsForConnectionLocked(
 const sp<Connection>& connection, const CancelationOptions& options) {
 if (connection->status == Connection::STATUS_BROKEN) {
 return;
 }

 nsecs_t currentTime = now();

 Vector<EventEntry*> cancelationEvents;
    connection->inputState.synthesizeCancelationEvents(currentTime,
            cancelationEvents, options);

 if (!cancelationEvents.isEmpty()) {
#if DEBUG_OUTBOUND_EVENT_DETAILS
        ALOGD("channel '%s' ~ Synthesized %d cancelation events to bring channel back in sync "
 "with reality: %s, mode=%d.",
                connection->getInputChannelName(), cancelationEvents.size(),
                options.reason, options.mode);
#endif
 for (size_t i = 0; i < cancelationEvents.size(); i++) {
 EventEntry* cancelationEventEntry = cancelationEvents.itemAt(i);
 switch (cancelationEventEntry->type) {
 case EventEntry::TYPE_KEY:
                logOutboundKeyDetailsLocked("cancel - ",
 static_cast<KeyEntry*>(cancelationEventEntry));
 break;
 case EventEntry::TYPE_MOTION:
                logOutboundMotionDetailsLocked("cancel - ",
 static_cast<MotionEntry*>(cancelationEventEntry));
 break;
 }

 InputTarget target;
            sp<InputWindowHandle> windowHandle = getWindowHandleLocked(connection->inputChannel);
 if (windowHandle != NULL) {
 const InputWindowInfo* windowInfo = windowHandle->getInfo();
                target.xOffset = -windowInfo->frameLeft;
                target.yOffset = -windowInfo->frameTop;
                target.scaleFactor = windowInfo->scaleFactor;
 } else {
                target.xOffset = 0;
                target.yOffset = 0;
                target.scaleFactor = 1.0f;
 }
            target.inputChannel = connection->inputChannel;
            target.flags = InputTarget::FLAG_DISPATCH_AS_IS;

            enqueueDispatchEntryLocked(connection, cancelationEventEntry,  
 &target, InputTarget::FLAG_DISPATCH_AS_IS);

            cancelationEventEntry->release();
 }

        startDispatchCycleLocked(currentTime, connection);
 }
}
