bool InputDispatcher::haveCommandsLocked() const {
 return !mCommandQueue.isEmpty();
}
