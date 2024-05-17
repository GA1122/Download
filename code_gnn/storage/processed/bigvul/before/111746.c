void PromoResourceService::ScheduleNotificationOnInit() {
  std::string locale = g_browser_process->GetApplicationLocale();
  if (GetPromoServiceVersion() != kPromoServiceVersion ||
      GetPromoLocale() != locale) {
    PrefService* local_state = g_browser_process->local_state();
    local_state->SetInteger(prefs::kNtpPromoVersion, kPromoServiceVersion);
    local_state->SetString(prefs::kNtpPromoLocale, locale);
    prefs_->ClearPref(prefs::kNtpPromoResourceCacheUpdate);
    PostNotification(0);
  } else {
    double promo_start = prefs_->GetDouble(prefs::kNtpPromoStart);
    double promo_end = prefs_->GetDouble(prefs::kNtpPromoEnd);
    ScheduleNotification(promo_start, promo_end);
  }
 }
