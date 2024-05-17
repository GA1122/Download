void InputDispatcher::synthesizeCancelationEventsForInputChannelLocked(
 const sp<InputChannel>& channel, const CancelationOptions& options) {
 ssize_t index = getConnectionIndexLocked(channel);
 if (index >= 0) {
        synthesizeCancelationEventsForConnectionLocked(
                mConnectionsByFd.valueAt(index), options);
 }
}
