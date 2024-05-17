void AcceleratorControllerDelegateAura::ShowDeprecatedAcceleratorNotification(
    const char* const notification_id,
    int message_id,
    int old_shortcut_id,
    int new_shortcut_id) {
  const base::string16 message =
      GetNotificationText(message_id, old_shortcut_id, new_shortcut_id);
  std::unique_ptr<message_center::Notification> notification(
      new message_center::Notification(
          message_center::NOTIFICATION_TYPE_SIMPLE, notification_id,
          base::string16(), message,
          WmShell::Get()->delegate()->GetDeprecatedAcceleratorImage(),
          base::string16(), GURL(),
          message_center::NotifierId(
              message_center::NotifierId::SYSTEM_COMPONENT,
              system_notifier::kNotifierDeprecatedAccelerator),
          message_center::RichNotificationData(),
          new DeprecatedAcceleratorNotificationDelegate));
  message_center::MessageCenter::Get()->AddNotification(
      std::move(notification));
}
