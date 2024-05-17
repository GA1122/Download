void InputDispatcher::InputState::addMotionMemento(const MotionEntry* entry,
 int32_t flags, bool hovering) {
    mMotionMementos.push();
 MotionMemento& memento = mMotionMementos.editTop();
    memento.deviceId = entry->deviceId;
    memento.source = entry->source;
    memento.flags = flags;
    memento.xPrecision = entry->xPrecision;
    memento.yPrecision = entry->yPrecision;
    memento.downTime = entry->downTime;
    memento.displayId = entry->displayId;
    memento.setPointers(entry);
    memento.hovering = hovering;
    memento.policyFlags = entry->policyFlags;
}
