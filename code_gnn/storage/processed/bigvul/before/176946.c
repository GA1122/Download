sp<InputWindowHandle> InputDispatcher::getWindowHandleLocked(
 const sp<InputChannel>& inputChannel) const {
 size_t numWindows = mWindowHandles.size();
 for (size_t i = 0; i < numWindows; i++) {
 const sp<InputWindowHandle>& windowHandle = mWindowHandles.itemAt(i);
 if (windowHandle->getInputChannel() == inputChannel) {
 return windowHandle;
 }
 }
 return NULL;
}
