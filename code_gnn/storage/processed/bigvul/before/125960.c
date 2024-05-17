InitialLoadObserver::InitialLoadObserver(size_t tab_count,
                                         AutomationProvider* automation)
    : automation_(automation->AsWeakPtr()),
      crashed_tab_count_(0),
      outstanding_tab_count_(tab_count),
      init_time_(base::TimeTicks::Now()) {
  if (outstanding_tab_count_ > 0) {
    registrar_.Add(this, content::NOTIFICATION_LOAD_START,
                   content::NotificationService::AllSources());
    registrar_.Add(this, content::NOTIFICATION_LOAD_STOP,
                   content::NotificationService::AllSources());
    registrar_.Add(this, content::NOTIFICATION_RENDERER_PROCESS_CLOSED,
                   content::NotificationService::AllSources());
  }
}
