bool IsAutofillUpstreamAlwaysRequestCardholderNameExperimentEnabled() {
  return base::FeatureList::IsEnabled(
      features::kAutofillUpstreamAlwaysRequestCardholderName);
}
