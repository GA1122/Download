  void SendProgressNotifications(const GURL& url,
                                 int num_total,
                                 int num_complete) {
    for (auto* frontend : hosts_to_notify_)
      frontend->ProgressEventRaised(url, num_total, num_complete);
  }
