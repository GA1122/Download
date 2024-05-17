void LowBatteryObserver::Show(base::TimeDelta remaining, bool urgent) {
  notification_.Show(l10n_util::GetStringFUTF16(IDS_LOW_BATTERY_MESSAGE,
      TimeFormat::TimeRemaining(remaining)), urgent, true);
  remaining_ = remaining.InMinutes();
}
