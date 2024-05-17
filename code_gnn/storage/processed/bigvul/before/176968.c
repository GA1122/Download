void InputDispatcher::logOutboundMotionDetailsLocked(const char* prefix, const MotionEntry* entry) {
#if DEBUG_OUTBOUND_EVENT_DETAILS
    ALOGD("%seventTime=%lld, deviceId=%d, source=0x%x, policyFlags=0x%x, "
 "action=0x%x, actionButton=0x%x, flags=0x%x, "
 "metaState=0x%x, buttonState=0x%x,"
 "edgeFlags=0x%x, xPrecision=%f, yPrecision=%f, downTime=%lld",
            prefix,
            entry->eventTime, entry->deviceId, entry->source, entry->policyFlags,
            entry->action, entry->actionButton, entry->flags,
            entry->metaState, entry->buttonState,
            entry->edgeFlags, entry->xPrecision, entry->yPrecision,
            entry->downTime);

 for (uint32_t i = 0; i < entry->pointerCount; i++) {
        ALOGD("  Pointer %d: id=%d, toolType=%d, "
 "x=%f, y=%f, pressure=%f, size=%f, "
 "touchMajor=%f, touchMinor=%f, toolMajor=%f, toolMinor=%f, "
 "orientation=%f",
                i, entry->pointerProperties[i].id,
                entry->pointerProperties[i].toolType,
                entry->pointerCoords[i].getAxisValue(AMOTION_EVENT_AXIS_X),
                entry->pointerCoords[i].getAxisValue(AMOTION_EVENT_AXIS_Y),
                entry->pointerCoords[i].getAxisValue(AMOTION_EVENT_AXIS_PRESSURE),
                entry->pointerCoords[i].getAxisValue(AMOTION_EVENT_AXIS_SIZE),
                entry->pointerCoords[i].getAxisValue(AMOTION_EVENT_AXIS_TOUCH_MAJOR),
                entry->pointerCoords[i].getAxisValue(AMOTION_EVENT_AXIS_TOUCH_MINOR),
                entry->pointerCoords[i].getAxisValue(AMOTION_EVENT_AXIS_TOOL_MAJOR),
                entry->pointerCoords[i].getAxisValue(AMOTION_EVENT_AXIS_TOOL_MINOR),
                entry->pointerCoords[i].getAxisValue(AMOTION_EVENT_AXIS_ORIENTATION));
 }
#endif
}
