sp<InputWindowHandle> InputDispatcher::findTouchedWindowAtLocked(int32_t displayId,
 int32_t x, int32_t y) {
 size_t numWindows = mWindowHandles.size();
 for (size_t i = 0; i < numWindows; i++) {
        sp<InputWindowHandle> windowHandle = mWindowHandles.itemAt(i);
 const InputWindowInfo* windowInfo = windowHandle->getInfo();
 if (windowInfo->displayId == displayId) {
 int32_t flags = windowInfo->layoutParamsFlags;

 if (windowInfo->visible) {
 if (!(flags & InputWindowInfo::FLAG_NOT_TOUCHABLE)) {
 bool isTouchModal = (flags & (InputWindowInfo::FLAG_NOT_FOCUSABLE
 | InputWindowInfo::FLAG_NOT_TOUCH_MODAL)) == 0;
 if (isTouchModal || windowInfo->touchableRegionContainsPoint(x, y)) {
 return windowHandle;
 }
 }
 }
 }
 }
 return NULL;
}
