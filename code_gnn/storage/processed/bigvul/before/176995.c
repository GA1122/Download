void InputDispatcher::resetAndDropEverythingLocked(const char* reason) {
#if DEBUG_FOCUS
    ALOGD("Resetting and dropping all events (%s).", reason);
#endif

 CancelationOptions options(CancelationOptions::CANCEL_ALL_EVENTS, reason);
    synthesizeCancelationEventsForAllConnectionsLocked(options);

    resetKeyRepeatLocked();
    releasePendingEventLocked();
    drainInboundQueueLocked();
    resetANRTimeoutsLocked();

    mTouchStatesByDisplay.clear();
    mLastHoverWindowHandle.clear();
    mReplacedKeys.clear();
}
