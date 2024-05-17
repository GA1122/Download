bool InputDispatcher::dispatchDeviceResetLocked(
 nsecs_t currentTime, DeviceResetEntry* entry) {
#if DEBUG_OUTBOUND_EVENT_DETAILS
    ALOGD("dispatchDeviceReset - eventTime=%lld, deviceId=%d", entry->eventTime, entry->deviceId);
#endif

 CancelationOptions options(CancelationOptions::CANCEL_ALL_EVENTS,
 "device was reset");
    options.deviceId = entry->deviceId;
    synthesizeCancelationEventsForAllConnectionsLocked(options);
 return true;
}
