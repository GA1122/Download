void AppCacheUpdateJob::NotifyAllProgress(const GURL& url) {
  HostNotifier host_notifier;
  AddAllAssociatedHostsToNotifier(&host_notifier);
  host_notifier.SendProgressNotifications(
      url, url_file_list_.size(), url_fetches_completed_);
}
