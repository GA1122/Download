InputDispatcher::MotionEntry::MotionEntry(nsecs_t eventTime, int32_t deviceId,
 uint32_t source, uint32_t policyFlags, int32_t action, int32_t actionButton,
 int32_t flags, int32_t metaState, int32_t buttonState, int32_t edgeFlags,
 float xPrecision, float yPrecision, nsecs_t downTime,
 int32_t displayId, uint32_t pointerCount,
 const PointerProperties* pointerProperties, const PointerCoords* pointerCoords,
 float xOffset, float yOffset) :
 EventEntry(TYPE_MOTION, eventTime, policyFlags),
        eventTime(eventTime),
        deviceId(deviceId), source(source), action(action), actionButton(actionButton),
        flags(flags), metaState(metaState), buttonState(buttonState),
        edgeFlags(edgeFlags), xPrecision(xPrecision), yPrecision(yPrecision),
        downTime(downTime), displayId(displayId), pointerCount(pointerCount) {
 for (uint32_t i = 0; i < pointerCount; i++) {
 this->pointerProperties[i].copyFrom(pointerProperties[i]);
 this->pointerCoords[i].copyFrom(pointerCoords[i]);
 if (xOffset || yOffset) {
 this->pointerCoords[i].applyOffset(xOffset, yOffset);
 }
 }
}
