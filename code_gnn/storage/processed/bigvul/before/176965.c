bool InputDispatcher::isWindowObscuredAtPointLocked(
 const sp<InputWindowHandle>& windowHandle, int32_t x, int32_t y) const {
 int32_t displayId = windowHandle->getInfo()->displayId;
 size_t numWindows = mWindowHandles.size();
 for (size_t i = 0; i < numWindows; i++) {
        sp<InputWindowHandle> otherHandle = mWindowHandles.itemAt(i);
 if (otherHandle == windowHandle) {
 break;
 }

 const InputWindowInfo* otherInfo = otherHandle->getInfo();
 if (otherInfo->displayId == displayId
 && otherInfo->visible && !otherInfo->isTrustedOverlay()
 && otherInfo->frameContainsPoint(x, y)) {
 return true;
 }
 }

     return false;
 }
