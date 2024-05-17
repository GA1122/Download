void InputDispatcher::pokeUserActivityLocked(const EventEntry* eventEntry) {
 if (mFocusedWindowHandle != NULL) {
 const InputWindowInfo* info = mFocusedWindowHandle->getInfo();
 if (info->inputFeatures & InputWindowInfo::INPUT_FEATURE_DISABLE_USER_ACTIVITY) {
#if DEBUG_DISPATCH_CYCLE
            ALOGD("Not poking user activity: disabled by window '%s'.", info->name.string());
#endif
 return;
 }
 }

 int32_t eventType = USER_ACTIVITY_EVENT_OTHER;
 switch (eventEntry->type) {
 case EventEntry::TYPE_MOTION: {
 const MotionEntry* motionEntry = static_cast<const MotionEntry*>(eventEntry);
 if (motionEntry->action == AMOTION_EVENT_ACTION_CANCEL) {
 return;
 }

 if (MotionEvent::isTouchEvent(motionEntry->source, motionEntry->action)) {
            eventType = USER_ACTIVITY_EVENT_TOUCH;
 }
 break;
 }
 case EventEntry::TYPE_KEY: {
 const KeyEntry* keyEntry = static_cast<const KeyEntry*>(eventEntry);
 if (keyEntry->flags & AKEY_EVENT_FLAG_CANCELED) {
 return;
 }
        eventType = USER_ACTIVITY_EVENT_BUTTON;
 break;
 }
 }

 CommandEntry* commandEntry = postCommandLocked(
 & InputDispatcher::doPokeUserActivityLockedInterruptible);
    commandEntry->eventTime = eventEntry->eventTime;
    commandEntry->userActivityEventType = eventType;
}
