bool InputDispatcher::shouldSendMotionToInputFilterLocked(const NotifyMotionArgs* args) {
 return mInputFilterEnabled && isMainDisplay(args->displayId);
}
