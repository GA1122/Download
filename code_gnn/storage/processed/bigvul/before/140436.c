bool ShouldNotifyInState(base::android::ApplicationState state) {
  switch (state) {
    case base::android::APPLICATION_STATE_UNKNOWN:
    case base::android::APPLICATION_STATE_HAS_RUNNING_ACTIVITIES:
      return false;
    case base::android::APPLICATION_STATE_HAS_PAUSED_ACTIVITIES:
    case base::android::APPLICATION_STATE_HAS_STOPPED_ACTIVITIES:
    case base::android::APPLICATION_STATE_HAS_DESTROYED_ACTIVITIES:
      return true;
  }
  NOTREACHED();
   return false;
 }
