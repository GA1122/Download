  void SendProgressNotifications(
      const GURL& url, int num_total, int num_complete) {
    for (NotifyHostMap::iterator it = hosts_to_notify.begin();
         it != hosts_to_notify.end(); ++it) {
      AppCacheFrontend* frontend = it->first;
      frontend->OnProgressEventRaised(it->second, url,
                                      num_total, num_complete);
    }
  }
