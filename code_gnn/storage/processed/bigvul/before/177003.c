void InputDispatcher::setInputWindows(const Vector<sp<InputWindowHandle> >& inputWindowHandles) {
#if DEBUG_FOCUS
    ALOGD("setInputWindows");
#endif
 {  
 AutoMutex _l(mLock);

 Vector<sp<InputWindowHandle> > oldWindowHandles = mWindowHandles;
        mWindowHandles = inputWindowHandles;

        sp<InputWindowHandle> newFocusedWindowHandle;
 bool foundHoveredWindow = false;
 for (size_t i = 0; i < mWindowHandles.size(); i++) {
 const sp<InputWindowHandle>& windowHandle = mWindowHandles.itemAt(i);
 if (!windowHandle->updateInfo() || windowHandle->getInputChannel() == NULL) {
                mWindowHandles.removeAt(i--);
 continue;
 }
 if (windowHandle->getInfo()->hasFocus) {
                newFocusedWindowHandle = windowHandle;
 }
 if (windowHandle == mLastHoverWindowHandle) {
                foundHoveredWindow = true;
 }
 }

 if (!foundHoveredWindow) {
            mLastHoverWindowHandle = NULL;
 }

 if (mFocusedWindowHandle != newFocusedWindowHandle) {
 if (mFocusedWindowHandle != NULL) {
#if DEBUG_FOCUS
                ALOGD("Focus left window: %s",
                        mFocusedWindowHandle->getName().string());
#endif
                sp<InputChannel> focusedInputChannel = mFocusedWindowHandle->getInputChannel();
 if (focusedInputChannel != NULL) {
 CancelationOptions options(CancelationOptions::CANCEL_NON_POINTER_EVENTS,
 "focus left window");
                    synthesizeCancelationEventsForInputChannelLocked(
                            focusedInputChannel, options);
 }
 }
 if (newFocusedWindowHandle != NULL) {
#if DEBUG_FOCUS
                ALOGD("Focus entered window: %s",
                        newFocusedWindowHandle->getName().string());
#endif
 }
            mFocusedWindowHandle = newFocusedWindowHandle;
 }

 for (size_t d = 0; d < mTouchStatesByDisplay.size(); d++) {
 TouchState& state = mTouchStatesByDisplay.editValueAt(d);
 for (size_t i = 0; i < state.windows.size(); i++) {
 TouchedWindow& touchedWindow = state.windows.editItemAt(i);
 if (!hasWindowHandleLocked(touchedWindow.windowHandle)) {
#if DEBUG_FOCUS
                    ALOGD("Touched window was removed: %s",
                            touchedWindow.windowHandle->getName().string());
#endif
                    sp<InputChannel> touchedInputChannel =
                            touchedWindow.windowHandle->getInputChannel();
 if (touchedInputChannel != NULL) {
 CancelationOptions options(CancelationOptions::CANCEL_POINTER_EVENTS,
 "touched window was removed");
                        synthesizeCancelationEventsForInputChannelLocked(
                                touchedInputChannel, options);
 }
                    state.windows.removeAt(i--);
 }
 }
 }

 for (size_t i = 0; i < oldWindowHandles.size(); i++) {
 const sp<InputWindowHandle>& oldWindowHandle = oldWindowHandles.itemAt(i);
 if (!hasWindowHandleLocked(oldWindowHandle)) {
#if DEBUG_FOCUS
                ALOGD("Window went away: %s", oldWindowHandle->getName().string());
#endif
                oldWindowHandle->releaseInfo();
 }
 }
 }  

    mLooper->wake();
}
