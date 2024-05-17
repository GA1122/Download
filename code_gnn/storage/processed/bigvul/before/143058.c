String BaseAudioContext::state() const {
  switch (context_state_) {
    case kSuspended:
      return "suspended";
    case kRunning:
      return "running";
    case kClosed:
      return "closed";
  }
  NOTREACHED();
  return "";
}
