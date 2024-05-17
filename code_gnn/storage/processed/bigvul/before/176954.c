int32_t InputDispatcher::injectInputEvent(const InputEvent* event, int32_t displayId,
 int32_t injectorPid, int32_t injectorUid, int32_t syncMode, int32_t timeoutMillis,
 uint32_t policyFlags) {
#if DEBUG_INBOUND_EVENT_DETAILS
    ALOGD("injectInputEvent - eventType=%d, injectorPid=%d, injectorUid=%d, "
 "syncMode=%d, timeoutMillis=%d, policyFlags=0x%08x",
            event->getType(), injectorPid, injectorUid, syncMode, timeoutMillis, policyFlags);
#endif

 nsecs_t endTime = now() + milliseconds_to_nanoseconds(timeoutMillis);

    policyFlags |= POLICY_FLAG_INJECTED;
 if (hasInjectionPermission(injectorPid, injectorUid)) {
        policyFlags |= POLICY_FLAG_TRUSTED;
 }

 EventEntry* firstInjectedEntry;
 EventEntry* lastInjectedEntry;
 switch (event->getType()) {
 case AINPUT_EVENT_TYPE_KEY: {
 const KeyEvent* keyEvent = static_cast<const KeyEvent*>(event);
 int32_t action = keyEvent->getAction();
 if (! validateKeyEvent(action)) {
 return INPUT_EVENT_INJECTION_FAILED;
 }

 int32_t flags = keyEvent->getFlags();
 if (flags & AKEY_EVENT_FLAG_VIRTUAL_HARD_KEY) {
            policyFlags |= POLICY_FLAG_VIRTUAL;
 }

 if (!(policyFlags & POLICY_FLAG_FILTERED)) {
            mPolicy->interceptKeyBeforeQueueing(keyEvent,   policyFlags);
 }

        mLock.lock();
        firstInjectedEntry = new KeyEntry(keyEvent->getEventTime(),
                keyEvent->getDeviceId(), keyEvent->getSource(),
                policyFlags, action, flags,
                keyEvent->getKeyCode(), keyEvent->getScanCode(), keyEvent->getMetaState(),
                keyEvent->getRepeatCount(), keyEvent->getDownTime());
        lastInjectedEntry = firstInjectedEntry;
 break;
 }

 case AINPUT_EVENT_TYPE_MOTION: {
 const MotionEvent* motionEvent = static_cast<const MotionEvent*>(event);
 int32_t action = motionEvent->getAction();
 size_t pointerCount = motionEvent->getPointerCount();
 const PointerProperties* pointerProperties = motionEvent->getPointerProperties();
 int32_t actionButton = motionEvent->getActionButton();
 if (! validateMotionEvent(action, actionButton, pointerCount, pointerProperties)) {
 return INPUT_EVENT_INJECTION_FAILED;
 }

 if (!(policyFlags & POLICY_FLAG_FILTERED)) {
 nsecs_t eventTime = motionEvent->getEventTime();
            mPolicy->interceptMotionBeforeQueueing(eventTime,   policyFlags);
 }

        mLock.lock();
 const nsecs_t* sampleEventTimes = motionEvent->getSampleEventTimes();
 const PointerCoords* samplePointerCoords = motionEvent->getSamplePointerCoords();
        firstInjectedEntry = new MotionEntry(*sampleEventTimes,
                motionEvent->getDeviceId(), motionEvent->getSource(), policyFlags,
                action, actionButton, motionEvent->getFlags(),
                motionEvent->getMetaState(), motionEvent->getButtonState(),
                motionEvent->getEdgeFlags(),
                motionEvent->getXPrecision(), motionEvent->getYPrecision(),
                motionEvent->getDownTime(), displayId,
 uint32_t(pointerCount), pointerProperties, samplePointerCoords,
                motionEvent->getXOffset(), motionEvent->getYOffset());
        lastInjectedEntry = firstInjectedEntry;
 for (size_t i = motionEvent->getHistorySize(); i > 0; i--) {
            sampleEventTimes += 1;
            samplePointerCoords += pointerCount;
 MotionEntry* nextInjectedEntry = new MotionEntry(*sampleEventTimes,
                    motionEvent->getDeviceId(), motionEvent->getSource(), policyFlags,
                    action, actionButton, motionEvent->getFlags(),
                    motionEvent->getMetaState(), motionEvent->getButtonState(),
                    motionEvent->getEdgeFlags(),
                    motionEvent->getXPrecision(), motionEvent->getYPrecision(),
                    motionEvent->getDownTime(), displayId,
 uint32_t(pointerCount), pointerProperties, samplePointerCoords,
                    motionEvent->getXOffset(), motionEvent->getYOffset());
            lastInjectedEntry->next = nextInjectedEntry;
            lastInjectedEntry = nextInjectedEntry;
 }
 break;
 }

 default:
        ALOGW("Cannot inject event of type %d", event->getType());
 return INPUT_EVENT_INJECTION_FAILED;
 }

 InjectionState* injectionState = new InjectionState(injectorPid, injectorUid);
 if (syncMode == INPUT_EVENT_INJECTION_SYNC_NONE) {
        injectionState->injectionIsAsync = true;
 }

    injectionState->refCount += 1;
    lastInjectedEntry->injectionState = injectionState;

 bool needWake = false;
 for (EventEntry* entry = firstInjectedEntry; entry != NULL; ) {
 EventEntry* nextEntry = entry->next;
        needWake |= enqueueInboundEventLocked(entry);
        entry = nextEntry;
 }

    mLock.unlock();

 if (needWake) {
        mLooper->wake();
 }

 int32_t injectionResult;
 {  
 AutoMutex _l(mLock);

 if (syncMode == INPUT_EVENT_INJECTION_SYNC_NONE) {
            injectionResult = INPUT_EVENT_INJECTION_SUCCEEDED;
 } else {
 for (;;) {
                injectionResult = injectionState->injectionResult;
 if (injectionResult != INPUT_EVENT_INJECTION_PENDING) {
 break;
 }

 nsecs_t remainingTimeout = endTime - now();
 if (remainingTimeout <= 0) {
#if DEBUG_INJECTION
                    ALOGD("injectInputEvent - Timed out waiting for injection result "
 "to become available.");
#endif
                    injectionResult = INPUT_EVENT_INJECTION_TIMED_OUT;
 break;
 }

                mInjectionResultAvailableCondition.waitRelative(mLock, remainingTimeout);
 }

 if (injectionResult == INPUT_EVENT_INJECTION_SUCCEEDED
 && syncMode == INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_FINISHED) {
 while (injectionState->pendingForegroundDispatches != 0) {
#if DEBUG_INJECTION
                    ALOGD("injectInputEvent - Waiting for %d pending foreground dispatches.",
                            injectionState->pendingForegroundDispatches);
#endif
 nsecs_t remainingTimeout = endTime - now();
 if (remainingTimeout <= 0) {
#if DEBUG_INJECTION
                    ALOGD("injectInputEvent - Timed out waiting for pending foreground "
 "dispatches to finish.");
#endif
                        injectionResult = INPUT_EVENT_INJECTION_TIMED_OUT;
 break;
 }

                    mInjectionSyncFinishedCondition.waitRelative(mLock, remainingTimeout);
 }
 }
 }

        injectionState->release();
 }  

#if DEBUG_INJECTION
    ALOGD("injectInputEvent - Finished with result %d.  "
 "injectorPid=%d, injectorUid=%d",
            injectionResult, injectorPid, injectorUid);
#endif

 return injectionResult;
}
