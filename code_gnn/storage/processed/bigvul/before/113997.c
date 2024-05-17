  void UpdateClockType(PrefService* service) {
    clock_type_ = service->GetBoolean(prefs::kUse24HourClock) ?
        base::k24HourClock : base::k12HourClock;
    ash::ClockObserver* observer = tray_->clock_observer();
    if (observer)
      observer->OnDateFormatChanged();
  }
