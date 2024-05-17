void InputDispatcher::TouchState::addOrUpdateWindow(const sp<InputWindowHandle>& windowHandle,
 int32_t targetFlags, BitSet32 pointerIds) {
 if (targetFlags & InputTarget::FLAG_SPLIT) {
        split = true;
 }

 for (size_t i = 0; i < windows.size(); i++) {
 TouchedWindow& touchedWindow = windows.editItemAt(i);
 if (touchedWindow.windowHandle == windowHandle) {
            touchedWindow.targetFlags |= targetFlags;
 if (targetFlags & InputTarget::FLAG_DISPATCH_AS_SLIPPERY_EXIT) {
                touchedWindow.targetFlags &= ~InputTarget::FLAG_DISPATCH_AS_IS;
 }
            touchedWindow.pointerIds.value |= pointerIds.value;
 return;
 }
 }

    windows.push();

 TouchedWindow& touchedWindow = windows.editTop();
    touchedWindow.windowHandle = windowHandle;
    touchedWindow.targetFlags = targetFlags;
    touchedWindow.pointerIds = pointerIds;
}
