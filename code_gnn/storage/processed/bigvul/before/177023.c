bool InputDispatcher::InputState::trackMotion(const MotionEntry* entry,
 int32_t action, int32_t flags) {
 int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;
 switch (actionMasked) {
 case AMOTION_EVENT_ACTION_UP:
 case AMOTION_EVENT_ACTION_CANCEL: {
 ssize_t index = findMotionMemento(entry, false  );
 if (index >= 0) {
            mMotionMementos.removeAt(index);
 return true;
 }
#if DEBUG_OUTBOUND_EVENT_DETAILS
        ALOGD("Dropping inconsistent motion up or cancel event: deviceId=%d, source=%08x, "
 "actionMasked=%d",
                entry->deviceId, entry->source, actionMasked);
#endif
 return false;
 }

 case AMOTION_EVENT_ACTION_DOWN: {
 ssize_t index = findMotionMemento(entry, false  );
 if (index >= 0) {
            mMotionMementos.removeAt(index);
 }
        addMotionMemento(entry, flags, false  );
 return true;
 }

 case AMOTION_EVENT_ACTION_POINTER_UP:
 case AMOTION_EVENT_ACTION_POINTER_DOWN:
 case AMOTION_EVENT_ACTION_MOVE: {
 if (entry->source & AINPUT_SOURCE_CLASS_NAVIGATION) {
 return true;
 }

 ssize_t index = findMotionMemento(entry, false  );

 if (entry->source & AINPUT_SOURCE_CLASS_JOYSTICK) {
 if (index >= 0) {
 if (entry->pointerCoords[0].isEmpty()) {
                    mMotionMementos.removeAt(index);
 } else {
 MotionMemento& memento = mMotionMementos.editItemAt(index);
                    memento.setPointers(entry);
 }
 } else if (!entry->pointerCoords[0].isEmpty()) {
                addMotionMemento(entry, flags, false  );
 }

 return true;
 }
 if (index >= 0) {
 MotionMemento& memento = mMotionMementos.editItemAt(index);
            memento.setPointers(entry);
 return true;
 }
#if DEBUG_OUTBOUND_EVENT_DETAILS
        ALOGD("Dropping inconsistent motion pointer up/down or move event: "
 "deviceId=%d, source=%08x, actionMasked=%d",
                entry->deviceId, entry->source, actionMasked);
#endif
 return false;
 }

 case AMOTION_EVENT_ACTION_HOVER_EXIT: {
 ssize_t index = findMotionMemento(entry, true  );
 if (index >= 0) {
            mMotionMementos.removeAt(index);
 return true;
 }
#if DEBUG_OUTBOUND_EVENT_DETAILS
        ALOGD("Dropping inconsistent motion hover exit event: deviceId=%d, source=%08x",
                entry->deviceId, entry->source);
#endif
 return false;
 }

 case AMOTION_EVENT_ACTION_HOVER_ENTER:
 case AMOTION_EVENT_ACTION_HOVER_MOVE: {
 ssize_t index = findMotionMemento(entry, true  );
 if (index >= 0) {
            mMotionMementos.removeAt(index);
 }
        addMotionMemento(entry, flags, true  );
 return true;
 }

 default:
 return true;
 }
}
