void InitialLoadObserver::Observe(int type,
                                  const content::NotificationSource& source,
                                  const content::NotificationDetails& details) {
  if (type == content::NOTIFICATION_LOAD_START) {
    if (outstanding_tab_count_ > loading_tabs_.size())
      loading_tabs_.insert(TabTimeMap::value_type(
          source.map_key(),
          TabTime(base::TimeTicks::Now())));
  } else if (type == content::NOTIFICATION_LOAD_STOP) {
    if (outstanding_tab_count_ > finished_tabs_.size()) {
      TabTimeMap::iterator iter = loading_tabs_.find(source.map_key());
      if (iter != loading_tabs_.end()) {
        finished_tabs_.insert(source.map_key());
        iter->second.set_stop_time(base::TimeTicks::Now());
      }
    }
  } else if (type == content::NOTIFICATION_RENDERER_PROCESS_CLOSED) {
    base::TerminationStatus status =
        content::Details<content::RenderProcessHost::RendererClosedDetails>(
            details)->status;
    switch (status) {
      case base::TERMINATION_STATUS_NORMAL_TERMINATION:
        break;

      case base::TERMINATION_STATUS_ABNORMAL_TERMINATION:
      case base::TERMINATION_STATUS_PROCESS_WAS_KILLED:
      case base::TERMINATION_STATUS_PROCESS_CRASHED:
        crashed_tab_count_++;
        break;

      case base::TERMINATION_STATUS_STILL_RUNNING:
        LOG(ERROR) << "Got RENDERER_PROCESS_CLOSED notification, "
                   << "but the process is still running. We may miss further "
                   << "crash notification, resulting in hangs.";
        break;

      default:
        LOG(ERROR) << "Unhandled termination status " << status;
        NOTREACHED();
        break;
    }
  } else {
    NOTREACHED();
  }

  if (finished_tabs_.size() + crashed_tab_count_ >= outstanding_tab_count_)
    ConditionMet();
}
