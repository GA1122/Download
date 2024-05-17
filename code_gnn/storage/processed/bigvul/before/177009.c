InputDispatcher::splitMotionEvent(const MotionEntry* originalMotionEntry, BitSet32 pointerIds) {
    ALOG_ASSERT(pointerIds.value != 0);

 uint32_t splitPointerIndexMap[MAX_POINTERS];
 PointerProperties splitPointerProperties[MAX_POINTERS];
 PointerCoords splitPointerCoords[MAX_POINTERS];

 uint32_t originalPointerCount = originalMotionEntry->pointerCount;
 uint32_t splitPointerCount = 0;

 for (uint32_t originalPointerIndex = 0; originalPointerIndex < originalPointerCount;
            originalPointerIndex++) {
 const PointerProperties& pointerProperties =
                originalMotionEntry->pointerProperties[originalPointerIndex];
 uint32_t pointerId = uint32_t(pointerProperties.id);
 if (pointerIds.hasBit(pointerId)) {
            splitPointerIndexMap[splitPointerCount] = originalPointerIndex;
            splitPointerProperties[splitPointerCount].copyFrom(pointerProperties);
            splitPointerCoords[splitPointerCount].copyFrom(
                    originalMotionEntry->pointerCoords[originalPointerIndex]);
            splitPointerCount += 1;
 }
 }

 if (splitPointerCount != pointerIds.count()) {
        ALOGW("Dropping split motion event because the pointer count is %d but "
 "we expected there to be %d pointers.  This probably means we received "
 "a broken sequence of pointer ids from the input device.",
                splitPointerCount, pointerIds.count());
 return NULL;
 }

 int32_t action = originalMotionEntry->action;
 int32_t maskedAction = action & AMOTION_EVENT_ACTION_MASK;
 if (maskedAction == AMOTION_EVENT_ACTION_POINTER_DOWN
 || maskedAction == AMOTION_EVENT_ACTION_POINTER_UP) {
 int32_t originalPointerIndex = getMotionEventActionPointerIndex(action);
 const PointerProperties& pointerProperties =
                originalMotionEntry->pointerProperties[originalPointerIndex];
 uint32_t pointerId = uint32_t(pointerProperties.id);
 if (pointerIds.hasBit(pointerId)) {
 if (pointerIds.count() == 1) {
                action = maskedAction == AMOTION_EVENT_ACTION_POINTER_DOWN
 ? AMOTION_EVENT_ACTION_DOWN : AMOTION_EVENT_ACTION_UP;
 } else {
 uint32_t splitPointerIndex = 0;
 while (pointerId != uint32_t(splitPointerProperties[splitPointerIndex].id)) {
                    splitPointerIndex += 1;
 }
                action = maskedAction | (splitPointerIndex
 << AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT);
 }
 } else {
            action = AMOTION_EVENT_ACTION_MOVE;
 }
 }

 MotionEntry* splitMotionEntry = new MotionEntry(
            originalMotionEntry->eventTime,
            originalMotionEntry->deviceId,
            originalMotionEntry->source,
            originalMotionEntry->policyFlags,
            action,
            originalMotionEntry->actionButton,
            originalMotionEntry->flags,
            originalMotionEntry->metaState,
            originalMotionEntry->buttonState,
            originalMotionEntry->edgeFlags,
            originalMotionEntry->xPrecision,
            originalMotionEntry->yPrecision,
            originalMotionEntry->downTime,
            originalMotionEntry->displayId,
            splitPointerCount, splitPointerProperties, splitPointerCoords, 0, 0);

 if (originalMotionEntry->injectionState) {
        splitMotionEntry->injectionState = originalMotionEntry->injectionState;
        splitMotionEntry->injectionState->refCount += 1;
 }

 return splitMotionEntry;
}
