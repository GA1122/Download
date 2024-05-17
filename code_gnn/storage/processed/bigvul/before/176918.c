void InputDispatcher::dispatchOnceInnerLocked(nsecs_t* nextWakeupTime) {
 nsecs_t currentTime = now();

 if (!mDispatchEnabled) {
        resetKeyRepeatLocked();
 }

 if (mDispatchFrozen) {
#if DEBUG_FOCUS
        ALOGD("Dispatch frozen.  Waiting some more.");
#endif
 return;
 }

 bool isAppSwitchDue = mAppSwitchDueTime <= currentTime;
 if (mAppSwitchDueTime < *nextWakeupTime) {
 *nextWakeupTime = mAppSwitchDueTime;
 }

 if (! mPendingEvent) {
 if (mInboundQueue.isEmpty()) {
 if (isAppSwitchDue) {
                resetPendingAppSwitchLocked(false);
                isAppSwitchDue = false;
 }

 if (mKeyRepeatState.lastKeyEntry) {
 if (currentTime >= mKeyRepeatState.nextRepeatTime) {
                    mPendingEvent = synthesizeKeyRepeatLocked(currentTime);
 } else {
 if (mKeyRepeatState.nextRepeatTime < *nextWakeupTime) {
 *nextWakeupTime = mKeyRepeatState.nextRepeatTime;
 }
 }
 }

 if (!mPendingEvent) {
 return;
 }
 } else {
            mPendingEvent = mInboundQueue.dequeueAtHead();
            traceInboundQueueLengthLocked();
 }

 if (mPendingEvent->policyFlags & POLICY_FLAG_PASS_TO_USER) {
            pokeUserActivityLocked(mPendingEvent);
 }

        resetANRTimeoutsLocked();
 }

    ALOG_ASSERT(mPendingEvent != NULL);
 bool done = false;
 DropReason dropReason = DROP_REASON_NOT_DROPPED;
 if (!(mPendingEvent->policyFlags & POLICY_FLAG_PASS_TO_USER)) {
        dropReason = DROP_REASON_POLICY;
 } else if (!mDispatchEnabled) {
        dropReason = DROP_REASON_DISABLED;
 }

 if (mNextUnblockedEvent == mPendingEvent) {
        mNextUnblockedEvent = NULL;
 }

 switch (mPendingEvent->type) {
 case EventEntry::TYPE_CONFIGURATION_CHANGED: {
 ConfigurationChangedEntry* typedEntry =
 static_cast<ConfigurationChangedEntry*>(mPendingEvent);
        done = dispatchConfigurationChangedLocked(currentTime, typedEntry);
        dropReason = DROP_REASON_NOT_DROPPED;  
 break;
 }

 case EventEntry::TYPE_DEVICE_RESET: {
 DeviceResetEntry* typedEntry =
 static_cast<DeviceResetEntry*>(mPendingEvent);
        done = dispatchDeviceResetLocked(currentTime, typedEntry);
        dropReason = DROP_REASON_NOT_DROPPED;  
 break;
 }

 case EventEntry::TYPE_KEY: {
 KeyEntry* typedEntry = static_cast<KeyEntry*>(mPendingEvent);
 if (isAppSwitchDue) {
 if (isAppSwitchKeyEventLocked(typedEntry)) {
                resetPendingAppSwitchLocked(true);
                isAppSwitchDue = false;
 } else if (dropReason == DROP_REASON_NOT_DROPPED) {
                dropReason = DROP_REASON_APP_SWITCH;
 }
 }
 if (dropReason == DROP_REASON_NOT_DROPPED
 && isStaleEventLocked(currentTime, typedEntry)) {
            dropReason = DROP_REASON_STALE;
 }
 if (dropReason == DROP_REASON_NOT_DROPPED && mNextUnblockedEvent) {
            dropReason = DROP_REASON_BLOCKED;
 }
        done = dispatchKeyLocked(currentTime, typedEntry, &dropReason, nextWakeupTime);
 break;
 }

 case EventEntry::TYPE_MOTION: {
 MotionEntry* typedEntry = static_cast<MotionEntry*>(mPendingEvent);
 if (dropReason == DROP_REASON_NOT_DROPPED && isAppSwitchDue) {
            dropReason = DROP_REASON_APP_SWITCH;
 }
 if (dropReason == DROP_REASON_NOT_DROPPED
 && isStaleEventLocked(currentTime, typedEntry)) {
            dropReason = DROP_REASON_STALE;
 }
 if (dropReason == DROP_REASON_NOT_DROPPED && mNextUnblockedEvent) {
            dropReason = DROP_REASON_BLOCKED;
 }
        done = dispatchMotionLocked(currentTime, typedEntry,
 &dropReason, nextWakeupTime);
 break;
 }

 default:
        ALOG_ASSERT(false);
 break;
 }

 if (done) {
 if (dropReason != DROP_REASON_NOT_DROPPED) {
            dropInboundEventLocked(mPendingEvent, dropReason);
 }
        mLastDropReason = dropReason;

        releasePendingEventLocked();
 *nextWakeupTime = LONG_LONG_MIN;  
 }
}
