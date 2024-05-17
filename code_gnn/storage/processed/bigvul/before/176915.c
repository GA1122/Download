bool InputDispatcher::dispatchKeyLocked(nsecs_t currentTime, KeyEntry* entry,
 DropReason* dropReason, nsecs_t* nextWakeupTime) {
 if (! entry->dispatchInProgress) {
 if (entry->repeatCount == 0
 && entry->action == AKEY_EVENT_ACTION_DOWN
 && (entry->policyFlags & POLICY_FLAG_TRUSTED)
 && (!(entry->policyFlags & POLICY_FLAG_DISABLE_KEY_REPEAT))) {
 if (mKeyRepeatState.lastKeyEntry
 && mKeyRepeatState.lastKeyEntry->keyCode == entry->keyCode) {
                entry->repeatCount = mKeyRepeatState.lastKeyEntry->repeatCount + 1;
                resetKeyRepeatLocked();
                mKeyRepeatState.nextRepeatTime = LONG_LONG_MAX;  
 } else {
                resetKeyRepeatLocked();
                mKeyRepeatState.nextRepeatTime = entry->eventTime + mConfig.keyRepeatTimeout;
 }
            mKeyRepeatState.lastKeyEntry = entry;
            entry->refCount += 1;
 } else if (! entry->syntheticRepeat) {
            resetKeyRepeatLocked();
 }

 if (entry->repeatCount == 1) {
            entry->flags |= AKEY_EVENT_FLAG_LONG_PRESS;
 } else {
            entry->flags &= ~AKEY_EVENT_FLAG_LONG_PRESS;
 }

        entry->dispatchInProgress = true;

        logOutboundKeyDetailsLocked("dispatchKey - ", entry);
 }

 if (entry->interceptKeyResult == KeyEntry::INTERCEPT_KEY_RESULT_TRY_AGAIN_LATER) {
 if (currentTime < entry->interceptKeyWakeupTime) {
 if (entry->interceptKeyWakeupTime < *nextWakeupTime) {
 *nextWakeupTime = entry->interceptKeyWakeupTime;
 }
 return false;  
 }
        entry->interceptKeyResult = KeyEntry::INTERCEPT_KEY_RESULT_UNKNOWN;
        entry->interceptKeyWakeupTime = 0;
 }

 if (entry->interceptKeyResult == KeyEntry::INTERCEPT_KEY_RESULT_UNKNOWN) {
 if (entry->policyFlags & POLICY_FLAG_PASS_TO_USER) {
 CommandEntry* commandEntry = postCommandLocked(
 & InputDispatcher::doInterceptKeyBeforeDispatchingLockedInterruptible);
 if (mFocusedWindowHandle != NULL) {
                commandEntry->inputWindowHandle = mFocusedWindowHandle;
 }
            commandEntry->keyEntry = entry;
            entry->refCount += 1;
 return false;  
 } else {
            entry->interceptKeyResult = KeyEntry::INTERCEPT_KEY_RESULT_CONTINUE;
 }
 } else if (entry->interceptKeyResult == KeyEntry::INTERCEPT_KEY_RESULT_SKIP) {
 if (*dropReason == DROP_REASON_NOT_DROPPED) {
 *dropReason = DROP_REASON_POLICY;
 }
 }

 if (*dropReason != DROP_REASON_NOT_DROPPED) {
        setInjectionResultLocked(entry, *dropReason == DROP_REASON_POLICY
 ? INPUT_EVENT_INJECTION_SUCCEEDED : INPUT_EVENT_INJECTION_FAILED);
 return true;
 }

 Vector<InputTarget> inputTargets;
 int32_t injectionResult = findFocusedWindowTargetsLocked(currentTime,
            entry, inputTargets, nextWakeupTime);
 if (injectionResult == INPUT_EVENT_INJECTION_PENDING) {
 return false;
 }

    setInjectionResultLocked(entry, injectionResult);
 if (injectionResult != INPUT_EVENT_INJECTION_SUCCEEDED) {
 return true;
 }

    addMonitoringTargetsLocked(inputTargets);

    dispatchEventLocked(currentTime, entry, inputTargets);
 return true;
}
