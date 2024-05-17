void Shell::OnCastingSessionStartedOrStopped(bool started) {
#if defined(OS_CHROMEOS)
  FOR_EACH_OBSERVER(ShellObserver, observers_,
                    OnCastingSessionStartedOrStopped(started));
#endif
}
