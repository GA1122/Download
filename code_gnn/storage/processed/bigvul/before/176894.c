void InputDispatcher::InputState::addKeyMemento(const KeyEntry* entry, int32_t flags) {
    mKeyMementos.push();
 KeyMemento& memento = mKeyMementos.editTop();
    memento.deviceId = entry->deviceId;
    memento.source = entry->source;
    memento.keyCode = entry->keyCode;
    memento.scanCode = entry->scanCode;
    memento.metaState = entry->metaState;
    memento.flags = flags;
    memento.downTime = entry->downTime;
    memento.policyFlags = entry->policyFlags;
}
