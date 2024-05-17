bool InputDispatcher::InputState::isHovering(int32_t deviceId, uint32_t source,
 int32_t displayId) const {
 for (size_t i = 0; i < mMotionMementos.size(); i++) {
 const MotionMemento& memento = mMotionMementos.itemAt(i);
 if (memento.deviceId == deviceId
 && memento.source == source
 && memento.displayId == displayId
 && memento.hovering) {
 return true;
 }
 }
 return false;
}
