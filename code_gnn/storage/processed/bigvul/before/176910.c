void InputDispatcher::InputState::copyPointerStateTo(InputState& other) const {
 for (size_t i = 0; i < mMotionMementos.size(); i++) {
 const MotionMemento& memento = mMotionMementos.itemAt(i);
 if (memento.source & AINPUT_SOURCE_CLASS_POINTER) {
 for (size_t j = 0; j < other.mMotionMementos.size(); ) {
 const MotionMemento& otherMemento = other.mMotionMementos.itemAt(j);
 if (memento.deviceId == otherMemento.deviceId
 && memento.source == otherMemento.source
 && memento.displayId == otherMemento.displayId) {
                    other.mMotionMementos.removeAt(j);
 } else {
                    j += 1;
 }
 }
            other.mMotionMementos.push(memento);
 }
 }
}
