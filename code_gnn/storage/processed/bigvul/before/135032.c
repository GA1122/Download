  void SendErrorNotifications(const AppCacheErrorDetails& details) {
    DCHECK(!details.message.empty());
    for (NotifyHostMap::iterator it = hosts_to_notify.begin();
         it != hosts_to_notify.end(); ++it) {
      AppCacheFrontend* frontend = it->first;
      frontend->OnErrorEventRaised(it->second, details);
    }
  }
