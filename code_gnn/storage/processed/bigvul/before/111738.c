bool PromoResourceService::CanShowNotificationPromo(Profile* profile) {
  NotificationPromo notification_promo(profile);
  notification_promo.InitFromPrefs();
  return notification_promo.CanShow();
}
