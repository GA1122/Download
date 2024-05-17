bool IsExperimentalInputViewEnabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableExperimentalInputViewFeatures);
}
