void SessionService::RecordSessionUpdateHistogramData(int type,
    base::TimeTicks* last_updated_time) {
  if (!last_updated_time->is_null()) {
    base::TimeDelta delta = base::TimeTicks::Now() - *last_updated_time;
    bool use_long_period = false;
    if (delta >= save_delay_in_mins_) {
      use_long_period = true;
    }
    switch (type) {
      case chrome::NOTIFICATION_SESSION_SERVICE_SAVED :
        RecordUpdatedSaveTime(delta, use_long_period);
        RecordUpdatedSessionNavigationOrTab(delta, use_long_period);
        break;
      case content::NOTIFICATION_TAB_CLOSED:
        RecordUpdatedTabClosed(delta, use_long_period);
        RecordUpdatedSessionNavigationOrTab(delta, use_long_period);
        break;
      case content::NOTIFICATION_NAV_LIST_PRUNED:
        RecordUpdatedNavListPruned(delta, use_long_period);
        RecordUpdatedSessionNavigationOrTab(delta, use_long_period);
        break;
      case content::NOTIFICATION_NAV_ENTRY_COMMITTED:
        RecordUpdatedNavEntryCommit(delta, use_long_period);
        RecordUpdatedSessionNavigationOrTab(delta, use_long_period);
        break;
      default:
        NOTREACHED() << "Bad type sent to RecordSessionUpdateHistogramData";
        break;
    }
  }
  (*last_updated_time) = base::TimeTicks::Now();
}
