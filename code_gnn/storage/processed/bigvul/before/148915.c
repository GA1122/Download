bool RenderFrameHostManager::ShouldTransitionCrossSite() {
  if (SiteIsolationPolicy::AreCrossProcessFramesPossible())
    return true;

  return !base::CommandLine::ForCurrentProcess()->HasSwitch(
             switches::kSingleProcess) &&
         !base::CommandLine::ForCurrentProcess()->HasSwitch(
             switches::kProcessPerTab);
}
