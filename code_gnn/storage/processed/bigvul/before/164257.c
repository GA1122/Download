void AppCacheUpdateJob::NotifyAllError(
    const blink::mojom::AppCacheErrorDetails& details) {
  HostNotifier host_notifier;
  AddAllAssociatedHostsToNotifier(&host_notifier);
  host_notifier.SendErrorNotifications(details);
}
