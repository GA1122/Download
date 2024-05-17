bool InputDispatcher::InputState::trackKey(const KeyEntry* entry,
 int32_t action, int32_t flags) {
 switch (action) {
 case AKEY_EVENT_ACTION_UP: {
 if (entry->flags & AKEY_EVENT_FLAG_FALLBACK) {
 for (size_t i = 0; i < mFallbackKeys.size(); ) {
 if (mFallbackKeys.valueAt(i) == entry->keyCode) {
                    mFallbackKeys.removeItemsAt(i);
 } else {
                    i += 1;
 }
 }
 }
 ssize_t index = findKeyMemento(entry);
 if (index >= 0) {
            mKeyMementos.removeAt(index);
 return true;
 }
  
 return true;
 }

 case AKEY_EVENT_ACTION_DOWN: {
 ssize_t index = findKeyMemento(entry);
 if (index >= 0) {
            mKeyMementos.removeAt(index);
 }
        addKeyMemento(entry, flags);
 return true;
 }

 default:
 return true;
 }
}
