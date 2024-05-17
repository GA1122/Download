void InputDispatcher::notifyKey(const NotifyKeyArgs* args) {
#if DEBUG_INBOUND_EVENT_DETAILS
    ALOGD("notifyKey - eventTime=%lld, deviceId=%d, source=0x%x, policyFlags=0x%x, action=0x%x, "
 "flags=0x%x, keyCode=0x%x, scanCode=0x%x, metaState=0x%x, downTime=%lld",
            args->eventTime, args->deviceId, args->source, args->policyFlags,
            args->action, args->flags, args->keyCode, args->scanCode,
            args->metaState, args->downTime);
#endif
 if (!validateKeyEvent(args->action)) {
 return;
 }

 uint32_t policyFlags = args->policyFlags;
 int32_t flags = args->flags;
 int32_t metaState = args->metaState;
 if ((policyFlags & POLICY_FLAG_VIRTUAL) || (flags & AKEY_EVENT_FLAG_VIRTUAL_HARD_KEY)) {
        policyFlags |= POLICY_FLAG_VIRTUAL;
        flags |= AKEY_EVENT_FLAG_VIRTUAL_HARD_KEY;
 }
 if (policyFlags & POLICY_FLAG_FUNCTION) {
        metaState |= AMETA_FUNCTION_ON;
 }

    policyFlags |= POLICY_FLAG_TRUSTED;

 int32_t keyCode = args->keyCode;
 if (metaState & AMETA_META_ON && args->action == AKEY_EVENT_ACTION_DOWN) {
 int32_t newKeyCode = AKEYCODE_UNKNOWN;
 if (keyCode == AKEYCODE_DEL) {
            newKeyCode = AKEYCODE_BACK;
 } else if (keyCode == AKEYCODE_ENTER) {
            newKeyCode = AKEYCODE_HOME;
 }
 if (newKeyCode != AKEYCODE_UNKNOWN) {
 AutoMutex _l(mLock);
 struct KeyReplacement replacement = {keyCode, args->deviceId};
            mReplacedKeys.add(replacement, newKeyCode);
            keyCode = newKeyCode;
            metaState &= ~AMETA_META_ON;
 }
 } else if (args->action == AKEY_EVENT_ACTION_UP) {
 AutoMutex _l(mLock);
 struct KeyReplacement replacement = {keyCode, args->deviceId};
 ssize_t index = mReplacedKeys.indexOfKey(replacement);
 if (index >= 0) {
            keyCode = mReplacedKeys.valueAt(index);
            mReplacedKeys.removeItemsAt(index);
            metaState &= ~AMETA_META_ON;
 }
 }

 KeyEvent event;
    event.initialize(args->deviceId, args->source, args->action,
            flags, keyCode, args->scanCode, metaState, 0,
            args->downTime, args->eventTime);

    mPolicy->interceptKeyBeforeQueueing(&event,   policyFlags);

 bool needWake;
 {  
        mLock.lock();

 if (shouldSendKeyToInputFilterLocked(args)) {
            mLock.unlock();

            policyFlags |= POLICY_FLAG_FILTERED;
 if (!mPolicy->filterInputEvent(&event, policyFlags)) {
 return;  
 }

            mLock.lock();
 }

 int32_t repeatCount = 0;
 KeyEntry* newEntry = new KeyEntry(args->eventTime,
                args->deviceId, args->source, policyFlags,
                args->action, flags, keyCode, args->scanCode,
                metaState, repeatCount, args->downTime);

        needWake = enqueueInboundEventLocked(newEntry);
        mLock.unlock();
 }  

 if (needWake) {
        mLooper->wake();
 }
}
