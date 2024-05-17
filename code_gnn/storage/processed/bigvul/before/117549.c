LowBatteryObserver::LowBatteryObserver(Profile* profile)
  : notification_(profile, "battery.chromeos",
                  IDR_NOTIFICATION_LOW_BATTERY,
                  l10n_util::GetStringUTF16(IDS_LOW_BATTERY_TITLE)),
    remaining_(0) {}
