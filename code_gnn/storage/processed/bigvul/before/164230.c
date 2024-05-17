  void AddHost(AppCacheHost* host) {
    hosts_to_notify_.insert(host->frontend());
  }
