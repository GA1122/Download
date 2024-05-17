void ResourceMessageFilter::OnChannelError() {
  NotificationService::current()->Notify(
      NotificationType::RESOURCE_MESSAGE_FILTER_SHUTDOWN,
      Source<ResourceMessageFilter>(this),
      NotificationService::NoDetails());
}
