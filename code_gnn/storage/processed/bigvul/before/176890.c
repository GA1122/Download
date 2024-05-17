InputDispatcher::KeyEntry::KeyEntry(nsecs_t eventTime,
 int32_t deviceId, uint32_t source, uint32_t policyFlags, int32_t action,
 int32_t flags, int32_t keyCode, int32_t scanCode, int32_t metaState,
 int32_t repeatCount, nsecs_t downTime) :
 EventEntry(TYPE_KEY, eventTime, policyFlags),
        deviceId(deviceId), source(source), action(action), flags(flags),
        keyCode(keyCode), scanCode(scanCode), metaState(metaState),
        repeatCount(repeatCount), downTime(downTime),
        syntheticRepeat(false), interceptKeyResult(KeyEntry::INTERCEPT_KEY_RESULT_UNKNOWN),
        interceptKeyWakeupTime(0) {
}
