const char* BackgroundStateToString(bool is_backgrounded) {
  if (is_backgrounded) {
    return "backgrounded";
  } else {
    return "foregrounded";
  }
}
