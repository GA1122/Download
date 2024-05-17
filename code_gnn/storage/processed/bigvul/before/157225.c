bool IsBackgroundSuspendEnabled(WebMediaPlayerDelegate* delegate) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableMediaSuspend))
    return false;
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableMediaSuspend))
    return true;

  return delegate->IsBackgroundMediaSuspendEnabled();
}
