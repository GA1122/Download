StatusBubble* Browser::GetStatusBubble() {
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kKioskMode))
    return NULL;
  return window_ ? window_->GetStatusBubble() : NULL;
}
