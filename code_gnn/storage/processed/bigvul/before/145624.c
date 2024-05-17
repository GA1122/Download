bool IsSecMetadataEnabled() {
  return base::FeatureList::IsEnabled(features::kSecMetadata) ||
         base::CommandLine::ForCurrentProcess()->HasSwitch(
             switches::kEnableExperimentalWebPlatformFeatures);
}
