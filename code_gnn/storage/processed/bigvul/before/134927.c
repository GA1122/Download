bool IsAccountManagerEnabled() {
  return base::FeatureList::IsEnabled(kAccountManager);
}
