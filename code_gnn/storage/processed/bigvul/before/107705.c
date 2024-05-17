void StatusBubbleGtk::Observe(NotificationType type,
                              const NotificationSource& source,
                              const NotificationDetails& details) {
  if (type == NotificationType::BROWSER_THEME_CHANGED) {
    UserChangedTheme();
  }
}
