void InputDispatcher::releaseInboundEventLocked(EventEntry* entry) {
 InjectionState* injectionState = entry->injectionState;
 if (injectionState && injectionState->injectionResult == INPUT_EVENT_INJECTION_PENDING) {
#if DEBUG_DISPATCH_CYCLE
        ALOGD("Injected inbound event was dropped.");
#endif
        setInjectionResultLocked(entry, INPUT_EVENT_INJECTION_FAILED);
 }
 if (entry == mNextUnblockedEvent) {
        mNextUnblockedEvent = NULL;
 }
    addRecentEventLocked(entry);
    entry->release();
}
