StatusBubble* Browser::GetStatusBubble() {
#if !defined(OS_MACOSX)
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kKioskMode))
    return NULL;
#endif
  return window_ ? window_->GetStatusBubble() : NULL;
}
