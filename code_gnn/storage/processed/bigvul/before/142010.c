bool IsAutofillUpstreamBlankCardholderNameFieldExperimentEnabled() {
  return base::FeatureList::IsEnabled(
      features::kAutofillUpstreamBlankCardholderNameField);
}
