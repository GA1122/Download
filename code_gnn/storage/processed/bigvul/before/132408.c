bool IsGestureTypingEnabled() {
  std::string keyboard_switch =
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kGestureTyping);
  return keyboard_switch == switches::kGestureTypingEnabled;
}
