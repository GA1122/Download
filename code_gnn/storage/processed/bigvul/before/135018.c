void AppCacheUpdateJob::NotifySingleHost(AppCacheHost* host,
                                         AppCacheEventID event_id) {
  std::vector<int> ids(1, host->host_id());
  host->frontend()->OnEventRaised(ids, event_id);
}
