ssize_t InputDispatcher::InputState::findMotionMemento(const MotionEntry* entry,
 bool hovering) const {
 for (size_t i = 0; i < mMotionMementos.size(); i++) {
 const MotionMemento& memento = mMotionMementos.itemAt(i);
 if (memento.deviceId == entry->deviceId
 && memento.source == entry->source
 && memento.displayId == entry->displayId
 && memento.hovering == hovering) {
 return i;
 }
 }
 return -1;
}
