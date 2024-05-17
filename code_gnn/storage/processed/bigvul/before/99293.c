void ResourceMessageFilter::OnFilterAdded(IPC::Channel* channel) {
  channel_ = channel;

  registrar_.Add(this, NotificationType::BLACKLIST_NONVISUAL_RESOURCE_BLOCKED,
                 NotificationService::AllSources());
}
