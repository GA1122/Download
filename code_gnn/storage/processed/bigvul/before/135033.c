  void SendLogMessage(const std::string& message) {
    for (NotifyHostMap::iterator it = hosts_to_notify.begin();
         it != hosts_to_notify.end(); ++it) {
      AppCacheFrontend* frontend = it->first;
      for (HostIds::iterator id = it->second.begin();
           id != it->second.end(); ++id) {
        frontend->OnLogMessage(*id, APPCACHE_LOG_WARNING, message);
      }
    }
  }
