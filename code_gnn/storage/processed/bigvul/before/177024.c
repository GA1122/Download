bool InputDispatcher::transferTouchFocus(const sp<InputChannel>& fromChannel,
 const sp<InputChannel>& toChannel) {
#if DEBUG_FOCUS
    ALOGD("transferTouchFocus: fromChannel=%s, toChannel=%s",
            fromChannel->getName().string(), toChannel->getName().string());
#endif
 {  
 AutoMutex _l(mLock);

        sp<InputWindowHandle> fromWindowHandle = getWindowHandleLocked(fromChannel);
        sp<InputWindowHandle> toWindowHandle = getWindowHandleLocked(toChannel);
 if (fromWindowHandle == NULL || toWindowHandle == NULL) {
#if DEBUG_FOCUS
            ALOGD("Cannot transfer focus because from or to window not found.");
#endif
 return false;
 }
 if (fromWindowHandle == toWindowHandle) {
#if DEBUG_FOCUS
            ALOGD("Trivial transfer to same window.");
#endif
 return true;
 }
 if (fromWindowHandle->getInfo()->displayId != toWindowHandle->getInfo()->displayId) {
#if DEBUG_FOCUS
            ALOGD("Cannot transfer focus because windows are on different displays.");
#endif
 return false;
 }

 bool found = false;
 for (size_t d = 0; d < mTouchStatesByDisplay.size(); d++) {
 TouchState& state = mTouchStatesByDisplay.editValueAt(d);
 for (size_t i = 0; i < state.windows.size(); i++) {
 const TouchedWindow& touchedWindow = state.windows[i];
 if (touchedWindow.windowHandle == fromWindowHandle) {
 int32_t oldTargetFlags = touchedWindow.targetFlags;
 BitSet32 pointerIds = touchedWindow.pointerIds;

                    state.windows.removeAt(i);

 int32_t newTargetFlags = oldTargetFlags
 & (InputTarget::FLAG_FOREGROUND
 | InputTarget::FLAG_SPLIT | InputTarget::FLAG_DISPATCH_AS_IS);
                    state.addOrUpdateWindow(toWindowHandle, newTargetFlags, pointerIds);

                    found = true;
 goto Found;
 }
 }
 }
Found:

 if (! found) {
#if DEBUG_FOCUS
            ALOGD("Focus transfer failed because from window did not have focus.");
#endif
 return false;
 }

 ssize_t fromConnectionIndex = getConnectionIndexLocked(fromChannel);
 ssize_t toConnectionIndex = getConnectionIndexLocked(toChannel);
 if (fromConnectionIndex >= 0 && toConnectionIndex >= 0) {
            sp<Connection> fromConnection = mConnectionsByFd.valueAt(fromConnectionIndex);
            sp<Connection> toConnection = mConnectionsByFd.valueAt(toConnectionIndex);

            fromConnection->inputState.copyPointerStateTo(toConnection->inputState);
 CancelationOptions options(CancelationOptions::CANCEL_POINTER_EVENTS,
 "transferring touch focus from this window to another window");
            synthesizeCancelationEventsForConnectionLocked(fromConnection, options);
 }

#if DEBUG_FOCUS
        logDispatchStateLocked();
#endif
 }  

    mLooper->wake();
 return true;
}
