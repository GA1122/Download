void SaveProfilesAndSignalEvent(CallStackProfiles* profiles,
                                WaitableEvent* event,
                                const CallStackProfiles& pending_profiles) {
  *profiles = pending_profiles;
  event->Signal();
 }
