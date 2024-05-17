void InputDispatcher::resetANRTimeoutsLocked() {
#if DEBUG_FOCUS
        ALOGD("Resetting ANR timeouts.");
#endif

    mInputTargetWaitCause = INPUT_TARGET_WAIT_CAUSE_NONE;
    mInputTargetWaitApplicationHandle.clear();
}
