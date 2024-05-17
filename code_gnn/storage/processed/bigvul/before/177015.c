void InputDispatcher::synthesizeCancelationEventsForMonitorsLocked(
 const CancelationOptions& options) {
 for (size_t i = 0; i < mMonitoringChannels.size(); i++) {
        synthesizeCancelationEventsForInputChannelLocked(mMonitoringChannels[i], options);
 }
}
