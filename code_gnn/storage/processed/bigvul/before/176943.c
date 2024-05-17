static inline int32_t getMotionEventActionPointerIndex(int32_t action) {
 return (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
 >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
}
