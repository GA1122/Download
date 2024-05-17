bool InputDispatcher::dispatchConfigurationChangedLocked(
 nsecs_t currentTime, ConfigurationChangedEntry* entry) {
#if DEBUG_OUTBOUND_EVENT_DETAILS
    ALOGD("dispatchConfigurationChanged - eventTime=%lld", entry->eventTime);
#endif

    resetKeyRepeatLocked();

 CommandEntry* commandEntry = postCommandLocked(
 & InputDispatcher::doNotifyConfigurationChangedInterruptible);
    commandEntry->eventTime = entry->eventTime;
 return true;
}
