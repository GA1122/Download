void InputDispatcher::dumpDispatchStateLocked(String8& dump) {
    dump.appendFormat(INDENT "DispatchEnabled: %d\n", mDispatchEnabled);
    dump.appendFormat(INDENT "DispatchFrozen: %d\n", mDispatchFrozen);

 if (mFocusedApplicationHandle != NULL) {
        dump.appendFormat(INDENT "FocusedApplication: name='%s', dispatchingTimeout=%0.3fms\n",
                mFocusedApplicationHandle->getName().string(),
                mFocusedApplicationHandle->getDispatchingTimeout(
                        DEFAULT_INPUT_DISPATCHING_TIMEOUT) / 1000000.0);
 } else {
        dump.append(INDENT "FocusedApplication: <null>\n");
 }
    dump.appendFormat(INDENT "FocusedWindow: name='%s'\n",
            mFocusedWindowHandle != NULL ? mFocusedWindowHandle->getName().string() : "<null>");

 if (!mTouchStatesByDisplay.isEmpty()) {
        dump.appendFormat(INDENT "TouchStatesByDisplay:\n");
 for (size_t i = 0; i < mTouchStatesByDisplay.size(); i++) {
 const TouchState& state = mTouchStatesByDisplay.valueAt(i);
            dump.appendFormat(INDENT2 "%d: down=%s, split=%s, deviceId=%d, source=0x%08x\n",
                    state.displayId, toString(state.down), toString(state.split),
                    state.deviceId, state.source);
 if (!state.windows.isEmpty()) {
                dump.append(INDENT3 "Windows:\n");
 for (size_t i = 0; i < state.windows.size(); i++) {
 const TouchedWindow& touchedWindow = state.windows[i];
                    dump.appendFormat(INDENT4 "%zu: name='%s', pointerIds=0x%0x, targetFlags=0x%x\n",
                            i, touchedWindow.windowHandle->getName().string(),
                            touchedWindow.pointerIds.value,
                            touchedWindow.targetFlags);
 }
 } else {
                dump.append(INDENT3 "Windows: <none>\n");
 }
 }
 } else {
        dump.append(INDENT "TouchStates: <no displays touched>\n");
 }

 if (!mWindowHandles.isEmpty()) {
        dump.append(INDENT "Windows:\n");
 for (size_t i = 0; i < mWindowHandles.size(); i++) {
 const sp<InputWindowHandle>& windowHandle = mWindowHandles.itemAt(i);
 const InputWindowInfo* windowInfo = windowHandle->getInfo();

            dump.appendFormat(INDENT2 "%zu: name='%s', displayId=%d, "
 "paused=%s, hasFocus=%s, hasWallpaper=%s, "
 "visible=%s, canReceiveKeys=%s, flags=0x%08x, type=0x%08x, layer=%d, "
 "frame=[%d,%d][%d,%d], scale=%f, "
 "touchableRegion=",
                    i, windowInfo->name.string(), windowInfo->displayId,
                    toString(windowInfo->paused),
                    toString(windowInfo->hasFocus),
                    toString(windowInfo->hasWallpaper),
                    toString(windowInfo->visible),
                    toString(windowInfo->canReceiveKeys),
                    windowInfo->layoutParamsFlags, windowInfo->layoutParamsType,
                    windowInfo->layer,
                    windowInfo->frameLeft, windowInfo->frameTop,
                    windowInfo->frameRight, windowInfo->frameBottom,
                    windowInfo->scaleFactor);
            dumpRegion(dump, windowInfo->touchableRegion);
            dump.appendFormat(", inputFeatures=0x%08x", windowInfo->inputFeatures);
            dump.appendFormat(", ownerPid=%d, ownerUid=%d, dispatchingTimeout=%0.3fms\n",
                    windowInfo->ownerPid, windowInfo->ownerUid,
                    windowInfo->dispatchingTimeout / 1000000.0);
 }
 } else {
        dump.append(INDENT "Windows: <none>\n");
 }

 if (!mMonitoringChannels.isEmpty()) {
        dump.append(INDENT "MonitoringChannels:\n");
 for (size_t i = 0; i < mMonitoringChannels.size(); i++) {
 const sp<InputChannel>& channel = mMonitoringChannels[i];
            dump.appendFormat(INDENT2 "%zu: '%s'\n", i, channel->getName().string());
 }
 } else {
        dump.append(INDENT "MonitoringChannels: <none>\n");
 }

 nsecs_t currentTime = now();

 if (!mRecentQueue.isEmpty()) {
        dump.appendFormat(INDENT "RecentQueue: length=%u\n", mRecentQueue.count());
 for (EventEntry* entry = mRecentQueue.head; entry; entry = entry->next) {
            dump.append(INDENT2);
            entry->appendDescription(dump);
            dump.appendFormat(", age=%0.1fms\n",
 (currentTime - entry->eventTime) * 0.000001f);
 }
 } else {
        dump.append(INDENT "RecentQueue: <empty>\n");
 }

 if (mPendingEvent) {
        dump.append(INDENT "PendingEvent:\n");
        dump.append(INDENT2);
        mPendingEvent->appendDescription(dump);
        dump.appendFormat(", age=%0.1fms\n",
 (currentTime - mPendingEvent->eventTime) * 0.000001f);
 } else {
        dump.append(INDENT "PendingEvent: <none>\n");
 }

 if (!mInboundQueue.isEmpty()) {
        dump.appendFormat(INDENT "InboundQueue: length=%u\n", mInboundQueue.count());
 for (EventEntry* entry = mInboundQueue.head; entry; entry = entry->next) {
            dump.append(INDENT2);
            entry->appendDescription(dump);
            dump.appendFormat(", age=%0.1fms\n",
 (currentTime - entry->eventTime) * 0.000001f);
 }
 } else {
        dump.append(INDENT "InboundQueue: <empty>\n");
 }

 if (!mReplacedKeys.isEmpty()) {
        dump.append(INDENT "ReplacedKeys:\n");
 for (size_t i = 0; i < mReplacedKeys.size(); i++) {
 const KeyReplacement& replacement = mReplacedKeys.keyAt(i);
 int32_t newKeyCode = mReplacedKeys.valueAt(i);
            dump.appendFormat(INDENT2 "%zu: originalKeyCode=%d, deviceId=%d, newKeyCode=%d\n",
                    i, replacement.keyCode, replacement.deviceId, newKeyCode);
 }
 } else {
        dump.append(INDENT "ReplacedKeys: <empty>\n");
 }

 if (!mConnectionsByFd.isEmpty()) {
        dump.append(INDENT "Connections:\n");
 for (size_t i = 0; i < mConnectionsByFd.size(); i++) {
 const sp<Connection>& connection = mConnectionsByFd.valueAt(i);
            dump.appendFormat(INDENT2 "%zu: channelName='%s', windowName='%s', "
 "status=%s, monitor=%s, inputPublisherBlocked=%s\n",
                    i, connection->getInputChannelName(), connection->getWindowName(),
                    connection->getStatusLabel(), toString(connection->monitor),
                    toString(connection->inputPublisherBlocked));

 if (!connection->outboundQueue.isEmpty()) {
                dump.appendFormat(INDENT3 "OutboundQueue: length=%u\n",
                        connection->outboundQueue.count());
 for (DispatchEntry* entry = connection->outboundQueue.head; entry;
                        entry = entry->next) {
                    dump.append(INDENT4);
                    entry->eventEntry->appendDescription(dump);
                    dump.appendFormat(", targetFlags=0x%08x, resolvedAction=%d, age=%0.1fms\n",
                            entry->targetFlags, entry->resolvedAction,
 (currentTime - entry->eventEntry->eventTime) * 0.000001f);
 }
 } else {
                dump.append(INDENT3 "OutboundQueue: <empty>\n");
 }

 if (!connection->waitQueue.isEmpty()) {
                dump.appendFormat(INDENT3 "WaitQueue: length=%u\n",
                        connection->waitQueue.count());
 for (DispatchEntry* entry = connection->waitQueue.head; entry;
                        entry = entry->next) {
                    dump.append(INDENT4);
                    entry->eventEntry->appendDescription(dump);
                    dump.appendFormat(", targetFlags=0x%08x, resolvedAction=%d, "
 "age=%0.1fms, wait=%0.1fms\n",
                            entry->targetFlags, entry->resolvedAction,
 (currentTime - entry->eventEntry->eventTime) * 0.000001f,
 (currentTime - entry->deliveryTime) * 0.000001f);
 }
 } else {
                dump.append(INDENT3 "WaitQueue: <empty>\n");
 }
 }
 } else {
        dump.append(INDENT "Connections: <none>\n");
 }

 if (isAppSwitchPendingLocked()) {
        dump.appendFormat(INDENT "AppSwitch: pending, due in %0.1fms\n",
 (mAppSwitchDueTime - now()) / 1000000.0);
 } else {
        dump.append(INDENT "AppSwitch: not pending\n");
 }

    dump.append(INDENT "Configuration:\n");
    dump.appendFormat(INDENT2 "KeyRepeatDelay: %0.1fms\n",
            mConfig.keyRepeatDelay * 0.000001f);
    dump.appendFormat(INDENT2 "KeyRepeatTimeout: %0.1fms\n",
            mConfig.keyRepeatTimeout * 0.000001f);
}
