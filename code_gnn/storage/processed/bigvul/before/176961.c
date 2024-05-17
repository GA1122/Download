bool InputDispatcher::TouchState::isSlippery() const {
 bool haveSlipperyForegroundWindow = false;
 for (size_t i = 0; i < windows.size(); i++) {
 const TouchedWindow& window = windows.itemAt(i);
 if (window.targetFlags & InputTarget::FLAG_FOREGROUND) {
 if (haveSlipperyForegroundWindow
 || !(window.windowHandle->getInfo()->layoutParamsFlags
 & InputWindowInfo::FLAG_SLIPPERY)) {
 return false;
 }
            haveSlipperyForegroundWindow = true;
 }
 }
 return haveSlipperyForegroundWindow;
}
