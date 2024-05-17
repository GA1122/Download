base::string16 ScreenLayoutObserverTest::GetDisplayNotificationAdditionalText()
    const {
  const message_center::Notification* notification = GetDisplayNotification();
  return notification ? notification->message() : base::string16();
}
