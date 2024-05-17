  NavigationMetricsData(base::TimeTicks start_time,
                        GURL url,
                        RestoreType restore_type)
      : start_time_(start_time), url_(url) {
    is_restoring_from_last_session_ =
        (restore_type == RestoreType::LAST_SESSION_EXITED_CLEANLY ||
         restore_type == RestoreType::LAST_SESSION_CRASHED);
  }
