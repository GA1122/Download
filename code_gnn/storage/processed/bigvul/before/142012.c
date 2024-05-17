bool IsPasswordManualFallbackEnabled() {
  return base::FeatureList::IsEnabled(kAutofillManualFallback);
}
