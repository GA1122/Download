  void OnGotSession(SessionService::Handle handle,
                    std::vector<SessionWindow*>* windows) {
    base::TimeDelta time_to_got_sessions =
        base::TimeTicks::Now() - restore_started_;
    UMA_HISTOGRAM_CUSTOM_TIMES(
        "SessionRestore.TimeToGotSessions",
        time_to_got_sessions,
        base::TimeDelta::FromMilliseconds(10),
        base::TimeDelta::FromSeconds(1000),
        100);
#if defined(OS_CHROMEOS)
    chromeos::BootTimesLoader::Get()->AddLoginTimeMarker(
        "SessionRestore-GotSession", false);
#endif
    if (synchronous_) {
      windows_.swap(*windows);
      MessageLoop::current()->QuitNow();
      return;
    }

    ProcessSessionWindows(windows);
  }
