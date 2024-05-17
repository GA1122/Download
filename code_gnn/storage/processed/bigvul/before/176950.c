bool InputDispatcher::hasWindowHandleLocked(
 const sp<InputWindowHandle>& windowHandle) const {
 size_t numWindows = mWindowHandles.size();
 for (size_t i = 0; i < numWindows; i++) {
 if (mWindowHandles.itemAt(i) == windowHandle) {
 return true;
 }
 }
 return false;
}
