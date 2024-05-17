void ResourceMessageFilter::Observe(NotificationType type,
                                    const NotificationSource &source,
                                    const NotificationDetails &details) {
  if (type == NotificationType::BLACKLIST_NONVISUAL_RESOURCE_BLOCKED) {
    BlacklistUI::OnNonvisualContentBlocked(
        Details<const URLRequest>(details).ptr());
  }
}
