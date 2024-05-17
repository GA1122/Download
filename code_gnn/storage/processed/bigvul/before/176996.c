void InputDispatcher::resetKeyRepeatLocked() {
 if (mKeyRepeatState.lastKeyEntry) {
        mKeyRepeatState.lastKeyEntry->release();
        mKeyRepeatState.lastKeyEntry = NULL;
 }
}
