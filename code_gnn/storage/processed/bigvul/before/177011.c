void InputDispatcher::InputState::synthesizeCancelationEvents(nsecs_t currentTime,
 Vector<EventEntry*>& outEvents, const CancelationOptions& options) {
 for (size_t i = 0; i < mKeyMementos.size(); i++) {
 const KeyMemento& memento = mKeyMementos.itemAt(i);
 if (shouldCancelKey(memento, options)) {
            outEvents.push(new KeyEntry(currentTime,
                    memento.deviceId, memento.source, memento.policyFlags,
                    AKEY_EVENT_ACTION_UP, memento.flags | AKEY_EVENT_FLAG_CANCELED,
                    memento.keyCode, memento.scanCode, memento.metaState, 0, memento.downTime));
 }
 }

 for (size_t i = 0; i < mMotionMementos.size(); i++) {
 const MotionMemento& memento = mMotionMementos.itemAt(i);
 if (shouldCancelMotion(memento, options)) {
            outEvents.push(new MotionEntry(currentTime,
                    memento.deviceId, memento.source, memento.policyFlags,
                    memento.hovering
 ? AMOTION_EVENT_ACTION_HOVER_EXIT
 : AMOTION_EVENT_ACTION_CANCEL,
                    memento.flags, 0, 0, 0, 0,
                    memento.xPrecision, memento.yPrecision, memento.downTime,
                    memento.displayId,
                    memento.pointerCount, memento.pointerProperties, memento.pointerCoords,
 0, 0));
 }
 }
}
