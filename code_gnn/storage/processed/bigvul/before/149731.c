FeatureInfo::FeatureInfo() {
  InitializeBasicState(base::CommandLine::InitializedForCurrentProcess()
                           ? base::CommandLine::ForCurrentProcess()
                           : nullptr);
}
