InputDispatcher::EventEntry::EventEntry(int32_t type, nsecs_t eventTime, uint32_t policyFlags) :
        refCount(1), type(type), eventTime(eventTime), policyFlags(policyFlags),
        injectionState(NULL), dispatchInProgress(false) {
}
