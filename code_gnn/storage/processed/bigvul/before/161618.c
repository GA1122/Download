String AudioHandler::GetChannelCountMode() {
  switch (new_channel_count_mode_) {
    case kMax:
      return "max";
    case kClampedMax:
      return "clamped-max";
    case kExplicit:
      return "explicit";
  }
  NOTREACHED();
  return "";
}
