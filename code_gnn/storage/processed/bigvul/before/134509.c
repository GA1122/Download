bool IsScrollEndEffectEnabled() {
  return CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
      switches::kScrollEndEffect) == "1";
}
