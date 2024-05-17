bool InputDispatcher::InputState::isNeutral() const {
 return mKeyMementos.isEmpty() && mMotionMementos.isEmpty();
}
