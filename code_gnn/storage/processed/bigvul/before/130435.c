  explicit StartupWatchDogThread(const base::TimeDelta& duration)
      : base::Watchdog(duration, "Startup watchdog thread", true) {
  }
