void AppCacheUpdateJob::NotifyAllAssociatedHosts(
    blink::mojom::AppCacheEventID event_id) {
  HostNotifier host_notifier;
  AddAllAssociatedHostsToNotifier(&host_notifier);
  host_notifier.SendNotifications(event_id);
}
