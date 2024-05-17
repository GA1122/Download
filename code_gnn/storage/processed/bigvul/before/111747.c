void PromoResourceService::Unpack(const DictionaryValue& parsed_json) {
  NotificationPromo notification_promo(profile_);
  notification_promo.InitFromJson(parsed_json);

  if (notification_promo.new_notification()) {
    ScheduleNotification(notification_promo.StartTimeForGroup(),
                         notification_promo.EndTime());
  }
}
