void InputDispatcher::releasePendingEventLocked() {
 if (mPendingEvent) {
        resetANRTimeoutsLocked();
        releaseInboundEventLocked(mPendingEvent);
        mPendingEvent = NULL;
 }
}
