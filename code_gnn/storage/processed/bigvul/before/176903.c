void InputDispatcher::DeviceResetEntry::appendDescription(String8& msg) const {
    msg.appendFormat("DeviceResetEvent(deviceId=%d), policyFlags=0x%08x",
            deviceId, policyFlags);
}
