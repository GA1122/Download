bool ProfilingProcessHost::ShouldProfileNewRenderer(
    content::RenderProcessHost* renderer) const {
  if (Profile::FromBrowserContext(renderer->GetBrowserContext())
          ->GetProfileType() == Profile::INCOGNITO_PROFILE) {
    return false;
  }

  if (mode() == Mode::kAll) {
    return true;
  } else if (mode() == Mode::kRendererSampling && !profiled_renderer_) {
    if (always_sample_for_tests_) {
      return true;
    }

    return (base::RandUint64() % 100000) < 33333;
  }

  return false;
}
