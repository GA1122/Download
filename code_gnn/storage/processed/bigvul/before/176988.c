void InputDispatcher::EventEntry::releaseInjectionState() {
 if (injectionState) {
        injectionState->release();
        injectionState = NULL;
 }
}
