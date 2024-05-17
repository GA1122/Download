bool InputDispatcher::dispatchMotionLocked(
 nsecs_t currentTime, MotionEntry* entry, DropReason* dropReason, nsecs_t* nextWakeupTime) {
 if (! entry->dispatchInProgress) {
        entry->dispatchInProgress = true;

        logOutboundMotionDetailsLocked("dispatchMotion - ", entry);
 }

 if (*dropReason != DROP_REASON_NOT_DROPPED) {
        setInjectionResultLocked(entry, *dropReason == DROP_REASON_POLICY
 ? INPUT_EVENT_INJECTION_SUCCEEDED : INPUT_EVENT_INJECTION_FAILED);
 return true;
 }

 bool isPointerEvent = entry->source & AINPUT_SOURCE_CLASS_POINTER;

 Vector<InputTarget> inputTargets;

 bool conflictingPointerActions = false;
 int32_t injectionResult;
 if (isPointerEvent) {
        injectionResult = findTouchedWindowTargetsLocked(currentTime,
                entry, inputTargets, nextWakeupTime, &conflictingPointerActions);
 } else {
        injectionResult = findFocusedWindowTargetsLocked(currentTime,
                entry, inputTargets, nextWakeupTime);
 }
 if (injectionResult == INPUT_EVENT_INJECTION_PENDING) {
 return false;
 }

    setInjectionResultLocked(entry, injectionResult);
 if (injectionResult != INPUT_EVENT_INJECTION_SUCCEEDED) {
 if (injectionResult != INPUT_EVENT_INJECTION_PERMISSION_DENIED) {
 CancelationOptions::Mode mode(isPointerEvent ?
 CancelationOptions::CANCEL_POINTER_EVENTS :
 CancelationOptions::CANCEL_NON_POINTER_EVENTS);
 CancelationOptions options(mode, "input event injection failed");
            synthesizeCancelationEventsForMonitorsLocked(options);
 }
 return true;
 }

 if (isMainDisplay(entry->displayId)) {
        addMonitoringTargetsLocked(inputTargets);
 }

 if (conflictingPointerActions) {
 CancelationOptions options(CancelationOptions::CANCEL_POINTER_EVENTS,
 "conflicting pointer actions");
        synthesizeCancelationEventsForAllConnectionsLocked(options);
 }
    dispatchEventLocked(currentTime, entry, inputTargets);
 return true;
}
