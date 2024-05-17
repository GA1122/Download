void ScreenLayoutObserver::CreateOrUpdateNotification(
    const base::string16& message,
    const base::string16& additional_message) {
  message_center::MessageCenter::Get()->RemoveNotification(kNotificationId,
                                                           false  );

  if (message.empty() && additional_message.empty())
    return;

  if (Shell::Get()
          ->screen_orientation_controller()
          ->ignore_display_configuration_updates()) {
    return;
  }

  ui::ResourceBundle& bundle = ui::ResourceBundle::GetSharedInstance();
  std::unique_ptr<Notification> notification(new Notification(
      message_center::NOTIFICATION_TYPE_SIMPLE, kNotificationId, message,
      additional_message, bundle.GetImageNamed(IDR_AURA_NOTIFICATION_DISPLAY),
      base::string16(),   
      GURL(),
      message_center::NotifierId(message_center::NotifierId::SYSTEM_COMPONENT,
                                 system_notifier::kNotifierDisplay),
      message_center::RichNotificationData(),
      new message_center::HandleNotificationClickedDelegate(
          base::Bind(&OpenSettingsFromNotification))));

  ShellPort::Get()->RecordUserMetricsAction(
      UMA_STATUS_AREA_DISPLAY_NOTIFICATION_CREATED);
  message_center::MessageCenter::Get()->AddNotification(
      std::move(notification));
}
