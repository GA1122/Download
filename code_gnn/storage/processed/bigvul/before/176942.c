sp<InputWindowHandle> InputDispatcher::TouchState::getFirstForegroundWindowHandle() const {
 for (size_t i = 0; i < windows.size(); i++) {
 const TouchedWindow& window = windows.itemAt(i);
 if (window.targetFlags & InputTarget::FLAG_FOREGROUND) {
 return window.windowHandle;
 }
 }
 return NULL;
}
