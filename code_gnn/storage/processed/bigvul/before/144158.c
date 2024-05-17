bool IsOzoneDrmMojo() {
  return base::FeatureList::IsEnabled(kEnableOzoneDrmMojo) ||
         !IsAshInBrowserProcess();
}
