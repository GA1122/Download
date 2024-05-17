int32_t InputDispatcher::InputState::getFallbackKey(int32_t originalKeyCode) {
 ssize_t index = mFallbackKeys.indexOfKey(originalKeyCode);
 return index >= 0 ? mFallbackKeys.valueAt(index) : -1;
}
