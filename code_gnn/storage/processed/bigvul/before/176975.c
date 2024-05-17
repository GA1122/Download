void InputDispatcher::notifySwitch(const NotifySwitchArgs* args) {
#if DEBUG_INBOUND_EVENT_DETAILS
    ALOGD("notifySwitch - eventTime=%lld, policyFlags=0x%x, switchValues=0x%08x, switchMask=0x%08x",
            args->eventTime, args->policyFlags,
            args->switchValues, args->switchMask);
#endif

 uint32_t policyFlags = args->policyFlags;
    policyFlags |= POLICY_FLAG_TRUSTED;
    mPolicy->notifySwitch(args->eventTime,
            args->switchValues, args->switchMask, policyFlags);
}
