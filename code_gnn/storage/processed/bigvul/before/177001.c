void InputDispatcher::setInjectionResultLocked(EventEntry* entry, int32_t injectionResult) {
 InjectionState* injectionState = entry->injectionState;
 if (injectionState) {
#if DEBUG_INJECTION
        ALOGD("Setting input event injection result to %d.  "
 "injectorPid=%d, injectorUid=%d",
                 injectionResult, injectionState->injectorPid, injectionState->injectorUid);
#endif

 if (injectionState->injectionIsAsync
 && !(entry->policyFlags & POLICY_FLAG_FILTERED)) {
 switch (injectionResult) {
 case INPUT_EVENT_INJECTION_SUCCEEDED:
                ALOGV("Asynchronous input event injection succeeded.");
 break;
 case INPUT_EVENT_INJECTION_FAILED:
                ALOGW("Asynchronous input event injection failed.");
 break;
 case INPUT_EVENT_INJECTION_PERMISSION_DENIED:
                ALOGW("Asynchronous input event injection permission denied.");
 break;
 case INPUT_EVENT_INJECTION_TIMED_OUT:
                ALOGW("Asynchronous input event injection timed out.");
 break;
 }
 }

        injectionState->injectionResult = injectionResult;
        mInjectionResultAvailableCondition.broadcast();
 }
}
