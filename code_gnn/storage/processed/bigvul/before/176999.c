void InputDispatcher::InputState::setFallbackKey(int32_t originalKeyCode,
 int32_t fallbackKeyCode) {
 ssize_t index = mFallbackKeys.indexOfKey(originalKeyCode);
 if (index >= 0) {
        mFallbackKeys.replaceValueAt(index, fallbackKeyCode);
 } else {
        mFallbackKeys.add(originalKeyCode, fallbackKeyCode);
 }
}
