static bool validateMotionEvent(int32_t action, int32_t actionButton, size_t pointerCount,
 const PointerProperties* pointerProperties) {
 if (! isValidMotionAction(action, actionButton, pointerCount)) {
        ALOGE("Motion event has invalid action code 0x%x", action);
 return false;
 }
 if (pointerCount < 1 || pointerCount > MAX_POINTERS) {
        ALOGE("Motion event has invalid pointer count %zu; value must be between 1 and %d.",
                pointerCount, MAX_POINTERS);
 return false;
 }
 BitSet32 pointerIdBits;
 for (size_t i = 0; i < pointerCount; i++) {
 int32_t id = pointerProperties[i].id;
 if (id < 0 || id > MAX_POINTER_ID) {
            ALOGE("Motion event has invalid pointer id %d; value must be between 0 and %d",
                    id, MAX_POINTER_ID);
 return false;
 }
 if (pointerIdBits.hasBit(id)) {
            ALOGE("Motion event has duplicate pointer id %d", id);
 return false;
 }
        pointerIdBits.markBit(id);
 }
 return true;
}
