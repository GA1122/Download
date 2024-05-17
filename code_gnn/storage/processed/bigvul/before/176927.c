void InputDispatcher::dropInboundEventLocked(EventEntry* entry, DropReason dropReason) {
 const char* reason;
 switch (dropReason) {
 case DROP_REASON_POLICY:
#if DEBUG_INBOUND_EVENT_DETAILS
        ALOGD("Dropped event because policy consumed it.");
#endif
        reason = "inbound event was dropped because the policy consumed it";
 break;
 case DROP_REASON_DISABLED:
 if (mLastDropReason != DROP_REASON_DISABLED) {
            ALOGI("Dropped event because input dispatch is disabled.");
 }
        reason = "inbound event was dropped because input dispatch is disabled";
 break;
 case DROP_REASON_APP_SWITCH:
        ALOGI("Dropped event because of pending overdue app switch.");
        reason = "inbound event was dropped because of pending overdue app switch";
 break;
 case DROP_REASON_BLOCKED:
        ALOGI("Dropped event because the current application is not responding and the user "
 "has started interacting with a different application.");
        reason = "inbound event was dropped because the current application is not responding "
 "and the user has started interacting with a different application";
 break;
 case DROP_REASON_STALE:
        ALOGI("Dropped event because it is stale.");
        reason = "inbound event was dropped because it is stale";
 break;
 default:
        ALOG_ASSERT(false);
 return;
 }

 switch (entry->type) {
 case EventEntry::TYPE_KEY: {
 CancelationOptions options(CancelationOptions::CANCEL_NON_POINTER_EVENTS, reason);
        synthesizeCancelationEventsForAllConnectionsLocked(options);
 break;
 }
 case EventEntry::TYPE_MOTION: {
 MotionEntry* motionEntry = static_cast<MotionEntry*>(entry);
 if (motionEntry->source & AINPUT_SOURCE_CLASS_POINTER) {
 CancelationOptions options(CancelationOptions::CANCEL_POINTER_EVENTS, reason);
            synthesizeCancelationEventsForAllConnectionsLocked(options);
 } else {
 CancelationOptions options(CancelationOptions::CANCEL_NON_POINTER_EVENTS, reason);
            synthesizeCancelationEventsForAllConnectionsLocked(options);
 }
 break;
 }
 }
}
