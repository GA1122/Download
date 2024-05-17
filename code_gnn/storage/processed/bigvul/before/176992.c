void InputDispatcher::TouchState::removeWindow(const sp<InputWindowHandle>& windowHandle) {
 for (size_t i = 0; i < windows.size(); i++) {
 if (windows.itemAt(i).windowHandle == windowHandle) {
            windows.removeAt(i);
 return;
 }
 }
}
