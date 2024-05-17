void AppCacheUpdateJob::LogConsoleMessageToAll(const std::string& message) {
  HostNotifier host_notifier;
  AddAllAssociatedHostsToNotifier(&host_notifier);
  host_notifier.SendLogMessage(message);
}
