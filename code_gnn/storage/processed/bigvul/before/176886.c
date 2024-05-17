InputDispatcher::InjectionState::InjectionState(int32_t injectorPid, int32_t injectorUid) :
        refCount(1),
        injectorPid(injectorPid), injectorUid(injectorUid),
        injectionResult(INPUT_EVENT_INJECTION_PENDING), injectionIsAsync(false),
        pendingForegroundDispatches(0) {
}
