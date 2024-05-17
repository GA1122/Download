void InputDispatcher::InjectionState::release() {
    refCount -= 1;
 if (refCount == 0) {
 delete this;
 } else {
        ALOG_ASSERT(refCount > 0);
 }
}
