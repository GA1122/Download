bool IsAutofillManualFallbackEnabled() {
   return base::FeatureList::IsEnabled(kAutofillManualFallbackPhaseTwo);
 }
