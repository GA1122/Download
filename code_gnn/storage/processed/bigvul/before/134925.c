bool AreExperimentalAccessibilityFeaturesEnabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      chromeos::switches::kEnableExperimentalAccessibilityFeatures);
}
