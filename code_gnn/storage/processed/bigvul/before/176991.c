void InputDispatcher::removeMonitorChannelLocked(const sp<InputChannel>& inputChannel) {
 for (size_t i = 0; i < mMonitoringChannels.size(); i++) {
 if (mMonitoringChannels[i] == inputChannel) {
             mMonitoringChannels.removeAt(i);
 break;
 }
 }
}
