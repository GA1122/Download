bool InputDispatcher::afterKeyEventLockedInterruptible(const sp<Connection>& connection,
 DispatchEntry* dispatchEntry, KeyEntry* keyEntry, bool handled) {
 if (!(keyEntry->flags & AKEY_EVENT_FLAG_FALLBACK)) {
 int32_t originalKeyCode = keyEntry->keyCode;
 int32_t fallbackKeyCode = connection->inputState.getFallbackKey(originalKeyCode);
 if (keyEntry->action == AKEY_EVENT_ACTION_UP) {
            connection->inputState.removeFallbackKey(originalKeyCode);
 }

 if (handled || !dispatchEntry->hasForegroundTarget()) {
 if (fallbackKeyCode != -1) {
#if DEBUG_OUTBOUND_EVENT_DETAILS
                ALOGD("Unhandled key event: Asking policy to cancel fallback action.  "
 "keyCode=%d, action=%d, repeatCount=%d, policyFlags=0x%08x",
                        keyEntry->keyCode, keyEntry->action, keyEntry->repeatCount,
                        keyEntry->policyFlags);
#endif
 KeyEvent event;
                initializeKeyEvent(&event, keyEntry);
                event.setFlags(event.getFlags() | AKEY_EVENT_FLAG_CANCELED);

                mLock.unlock();

                mPolicy->dispatchUnhandledKey(connection->inputWindowHandle,
 &event, keyEntry->policyFlags, &event);

                mLock.lock();

 if (fallbackKeyCode != AKEYCODE_UNKNOWN) {
 CancelationOptions options(CancelationOptions::CANCEL_FALLBACK_EVENTS,
 "application handled the original non-fallback key "
 "or is no longer a foreground target, "
 "canceling previously dispatched fallback key");
                    options.keyCode = fallbackKeyCode;
                    synthesizeCancelationEventsForConnectionLocked(connection, options);
 }
                connection->inputState.removeFallbackKey(originalKeyCode);
 }
 } else {
 bool initialDown = keyEntry->action == AKEY_EVENT_ACTION_DOWN
 && keyEntry->repeatCount == 0;
 if (fallbackKeyCode == -1 && !initialDown) {
#if DEBUG_OUTBOUND_EVENT_DETAILS
                ALOGD("Unhandled key event: Skipping unhandled key event processing "
 "since this is not an initial down.  "
 "keyCode=%d, action=%d, repeatCount=%d, policyFlags=0x%08x",
                        originalKeyCode, keyEntry->action, keyEntry->repeatCount,
                        keyEntry->policyFlags);
#endif
 return false;
 }

#if DEBUG_OUTBOUND_EVENT_DETAILS
            ALOGD("Unhandled key event: Asking policy to perform fallback action.  "
 "keyCode=%d, action=%d, repeatCount=%d, policyFlags=0x%08x",
                    keyEntry->keyCode, keyEntry->action, keyEntry->repeatCount,
                    keyEntry->policyFlags);
#endif
 KeyEvent event;
            initializeKeyEvent(&event, keyEntry);

            mLock.unlock();

 bool fallback = mPolicy->dispatchUnhandledKey(connection->inputWindowHandle,
 &event, keyEntry->policyFlags, &event);

            mLock.lock();

 if (connection->status != Connection::STATUS_NORMAL) {
                connection->inputState.removeFallbackKey(originalKeyCode);
 return false;
 }

 if (initialDown) {
 if (fallback) {
                    fallbackKeyCode = event.getKeyCode();
 } else {
                    fallbackKeyCode = AKEYCODE_UNKNOWN;
 }
                connection->inputState.setFallbackKey(originalKeyCode, fallbackKeyCode);
 }

            ALOG_ASSERT(fallbackKeyCode != -1);

 if (fallbackKeyCode != AKEYCODE_UNKNOWN
 && (!fallback || fallbackKeyCode != event.getKeyCode())) {
#if DEBUG_OUTBOUND_EVENT_DETAILS
 if (fallback) {
                    ALOGD("Unhandled key event: Policy requested to send key %d"
 "as a fallback for %d, but on the DOWN it had requested "
 "to send %d instead.  Fallback canceled.",
                            event.getKeyCode(), originalKeyCode, fallbackKeyCode);
 } else {
                    ALOGD("Unhandled key event: Policy did not request fallback for %d, "
 "but on the DOWN it had requested to send %d.  "
 "Fallback canceled.",
                            originalKeyCode, fallbackKeyCode);
 }
#endif

 CancelationOptions options(CancelationOptions::CANCEL_FALLBACK_EVENTS,
 "canceling fallback, policy no longer desires it");
                options.keyCode = fallbackKeyCode;
                synthesizeCancelationEventsForConnectionLocked(connection, options);

                fallback = false;
                fallbackKeyCode = AKEYCODE_UNKNOWN;
 if (keyEntry->action != AKEY_EVENT_ACTION_UP) {
                    connection->inputState.setFallbackKey(originalKeyCode,
                            fallbackKeyCode);
 }
 }

#if DEBUG_OUTBOUND_EVENT_DETAILS
 {
 String8 msg;
 const KeyedVector<int32_t, int32_t>& fallbackKeys =
                        connection->inputState.getFallbackKeys();
 for (size_t i = 0; i < fallbackKeys.size(); i++) {
                    msg.appendFormat(", %d->%d", fallbackKeys.keyAt(i),
                            fallbackKeys.valueAt(i));
 }
                ALOGD("Unhandled key event: %d currently tracked fallback keys%s.",
                        fallbackKeys.size(), msg.string());
 }
#endif

 if (fallback) {
                keyEntry->eventTime = event.getEventTime();
                keyEntry->deviceId = event.getDeviceId();
                keyEntry->source = event.getSource();
                keyEntry->flags = event.getFlags() | AKEY_EVENT_FLAG_FALLBACK;
                keyEntry->keyCode = fallbackKeyCode;
                keyEntry->scanCode = event.getScanCode();
                keyEntry->metaState = event.getMetaState();
                keyEntry->repeatCount = event.getRepeatCount();
                keyEntry->downTime = event.getDownTime();
                keyEntry->syntheticRepeat = false;

#if DEBUG_OUTBOUND_EVENT_DETAILS
                ALOGD("Unhandled key event: Dispatching fallback key.  "
 "originalKeyCode=%d, fallbackKeyCode=%d, fallbackMetaState=%08x",
                        originalKeyCode, fallbackKeyCode, keyEntry->metaState);
#endif
 return true;  
 } else {
#if DEBUG_OUTBOUND_EVENT_DETAILS
                ALOGD("Unhandled key event: No fallback key.");
#endif
 }
 }
 }
 return false;
}
