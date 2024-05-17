bool RenderViewHostManager::ShouldTransitionCrossSite() {
  return !CommandLine::ForCurrentProcess()->HasSwitch(switches::kProcessPerTab);
}
