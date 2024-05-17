InputDispatcher::DeviceResetEntry::DeviceResetEntry(nsecs_t eventTime, int32_t deviceId) :
 EventEntry(TYPE_DEVICE_RESET, eventTime, 0),
        deviceId(deviceId) {
}
